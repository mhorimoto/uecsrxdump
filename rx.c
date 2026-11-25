#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define False 0
#define True  1

volatile sig_atomic_t stopflag = 0;
void abrt_handler(int sig);

#include "ccm.h"

int main(int argc, char* argv[]) {
  const char* optstring = "vmsti:c:";
  int sd;
  struct sockaddr_in addr;
 
  struct sigaction sa;

  char buf[2048];  // 受信バッファ
  char dbuf[2048]; // 表示バッファ
  char ipa[64];    // IPアドレスのマッチング
  char ccm[128];   // CCMのマッチング
  int  n;
  char ddd[11],tod[9];
  int  cnt;        // 受信カウンタ
  int  rc;
  int  c;
  int  opt_m,opt_s,opt_t,opt_i,opt_c;
  u_int yes = 1;
  time_t now;
  struct tm *tm_now;

  opt_m = False;
  opt_s = False;
  opt_t = False;
  opt_i = False;
  opt_c = False;
  
  while((c=getopt(argc,argv,optstring))!=-1) {
    //    printf("opt=%c ",c);
    if (c=='m') {
      opt_m = True;
    } else if (c=='s') {
      opt_s = True;
    } else if (c=='t') {
      opt_t = True;
    } else if (c=='i') {
      opt_i = True;
      n = snprintf(ipa,sizeof(ipa),"<IP>%s</IP>",optarg);
      if (n < 0 || (size_t)n >= sizeof(ipa)) {
	fprintf(stderr,"uecsrxdump: IP address too long: %s\n",optarg);
	exit(1);
      }
    } else if (c=='c') {
      opt_c = True;
      n = snprintf(ccm,sizeof(ccm),"type=\"%s\"",optarg);
      if (n < 0 || (size_t)n >= sizeof(ccm)) {
	fprintf(stderr,"uecsrxdump: CCM name too long: %s\n",optarg);
	exit(1);
      }
    } else if (c=='v') {
      printf("uecsrxdump UECS CCM packet Dump Version 1.20\n");
      exit(0);
    }
  }
  
  // SIGINTでブロック中のrecvfromを中断させるため、SA_RESTARTを付けずに登録する
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = abrt_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if ( sigaction(SIGINT, &sa, NULL) < 0 ) {
    perror("sigaction");
    exit(1);
  }

  // IPv4 UDP のソケットを作成
  if((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    return -1;
  }
 
  // 待ち受けるIPとポート番号を設定
  addr.sin_family = AF_INET;
  addr.sin_port = htons(16520);
  addr.sin_addr.s_addr = INADDR_ANY; // すべてのアドレス宛のパケットを受信する
 
  // バインドする
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  if(bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    return -1;
  }
 
  // 受信バッファの初期化
  memset(buf, 0, sizeof(buf));

  while(!stopflag) {
    // 受信 パケットが到着するまでブロック
    // 終端文字の分として1バイト残して受信する
    rc = recvfrom(sd, buf, sizeof(buf) - 1, 0, NULL, NULL);
    if ( rc > 0 ) {
      buf[rc] = '\0';
      if (opt_i) {
	if (strstr(buf,ipa)==NULL) continue;
      }
      if (opt_c) {
	if (strstr(buf,ccm)==NULL) continue;
      }
      // 日時生成
      if (opt_t) {
	now = time(NULL);
	tm_now = localtime(&now);
	sprintf(ddd,"%04d-%02d-%02d",tm_now->tm_year+1900,tm_now->tm_mon+1,tm_now->tm_mday);
	sprintf(tod,"%02d:%02d:%02d",tm_now->tm_hour,tm_now->tm_min,tm_now->tm_sec);
      }
      // 出力形式の変換 (UECSのバージョンや属性の順序に依存しない)
      if (opt_s) {
	if (ccm_to_csv(dbuf,sizeof(dbuf),buf) < 0) continue; // DATA以外はCSVにできないので出力しない
      } else if (opt_m) {
	ccm_strip(dbuf,sizeof(dbuf),buf);
      } else {
	strcpy(dbuf,buf);
      }
      // 受信データの出力
      if (opt_t) {
	printf("%s %s %s\n",ddd,tod,dbuf);
      } else {
	printf("%s\n",dbuf);
      }
    } else if ( rc < 0 ) {
      if ( errno == EINTR ) continue; // シグナル受信時はループ条件で終了判定
      perror("recvfrom");
    }
    // rc == 0 (空のデータグラム) は無視する
  }
  // ソケットのクローズ
  close(sd);
  return 0;
}

void abrt_handler(int sig) {
  stopflag = 1;
}
