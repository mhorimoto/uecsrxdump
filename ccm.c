#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ccm.h"

// [start, end) の範囲を out にコピーする (outsz を超える分は切り詰める)
static void copy_range(char *out, size_t outsz, const char *start, const char *end) {
  size_t len = (size_t)(end - start);
  if (outsz == 0) return;
  if (len >= outsz) len = outsz - 1;
  memcpy(out, start, len);
  out[len] = '\0';
}

// タグ [tag, tag_end) の中から name="..." の値を取り出す
// 属性の順序やバージョンに依存しない。見つからなければ -1
static int get_attr(const char *tag, const char *tag_end, const char *name,
		    char *out, size_t outsz) {
  size_t nlen = strlen(name);
  const char *s = tag;
  const char *v, *e;

  while ((s = strstr(s, name)) != NULL && s < tag_end) {
    if (s > tag && isspace((unsigned char)s[-1]) &&
	s[nlen] == '=' && s[nlen + 1] == '"') {
      v = s + nlen + 2;
      e = strchr(v, '"');
      if (e == NULL || e > tag_end) return -1;
      copy_range(out, outsz, v, e);
      return 0;
    }
    s++;
  }
  return -1;
}

// -m: <?xml ...?><UECS ...> と </UECS> を取り除く
// UECSの外枠が見つからなければ受信データをそのままコピーする
void ccm_strip(char *out, size_t outsz, const char *in) {
  const char *p, *start, *end;

  p = strstr(in, "<UECS");
  if (p == NULL || (start = strchr(p, '>')) == NULL) {
    copy_range(out, outsz, in, in + strlen(in));
    return;
  }
  start++;
  end = strstr(start, "</UECS>");
  if (end == NULL) end = start + strlen(start);
  copy_range(out, outsz, start, end);
}

// -s: DATAパケットを type,room,region,order,priority,値,IP のCSVに変換する
// DATAパケットでなければ -1 を返す
int ccm_to_csv(char *out, size_t outsz, const char *in) {
  const char *tag, *tag_end, *vend, *ip, *ipend;
  char type[256], room[16], region[16], order[16], priority[16];
  char value[1024], ipaddr[64];
  int n;

  tag = strstr(in, "<DATA");
  if (tag == NULL || !isspace((unsigned char)tag[5])) return -1;
  tag_end = strchr(tag, '>');
  if (tag_end == NULL) return -1;
  vend = strstr(tag_end, "</DATA>");
  if (vend == NULL) return -1;

  if (get_attr(tag, tag_end, "type", type, sizeof(type)) < 0) return -1;
  if (get_attr(tag, tag_end, "room", room, sizeof(room)) < 0) room[0] = '\0';
  if (get_attr(tag, tag_end, "region", region, sizeof(region)) < 0) region[0] = '\0';
  if (get_attr(tag, tag_end, "order", order, sizeof(order)) < 0) order[0] = '\0';
  if (get_attr(tag, tag_end, "priority", priority, sizeof(priority)) < 0) priority[0] = '\0';
  copy_range(value, sizeof(value), tag_end + 1, vend);

  ipaddr[0] = '\0';
  ip = strstr(vend, "<IP>");
  if (ip != NULL) {
    ip += 4;
    ipend = strstr(ip, "</IP>");
    if (ipend != NULL) copy_range(ipaddr, sizeof(ipaddr), ip, ipend);
  }

  n = snprintf(out, outsz, "%s,%s,%s,%s,%s,%s,%s",
	       type, room, region, order, priority, value, ipaddr);
  if (n < 0 || (size_t)n >= outsz) return -1;
  return 0;
}
