#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// rep関数（文字列置換関数）※マルチバイト文字ではエラー生じる場合あり
int rep(char *buf, const char *str, const char *repbef, const char *repaft) {
  char *pos; //strstr関数で使用するポインタ
  char *poss = (char *)str; //検索開始のポインタの位置（possとは[pos + start]の意）
  int len; //検索開始ポインタ（poss）から検索結果ポインタ（pos）までの長さ
  int beflen = strlen(repbef); //検索語句の長さ
  strcpy(buf, "\0"); //strcpyは第2引数のNULL終端を含めコピーする（バッファオーバーランには注意）
  
  while(1) {
    pos = (char *)strstr(poss, repbef);
    if (pos == NULL) {
      strcat(buf, poss); //strcatもNULL終端する（バッファオーバーランには注意）
      break;
    } else {
      strncat(buf, poss, pos - poss); //strncatもNULL終端する（バッファオーバーランには注意）
      strcat(buf, repaft); //strcatもNULL終端する（バッファオーバーランには注意）
      poss = pos + beflen;
    }
  }
  return 0;
}
