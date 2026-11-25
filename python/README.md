# uecsrxdump (Python版)

UECS用CCM受電データダンププログラムのPython版

UECSの16520/udpのパケットを受電して整形して標準出力に出力する。
C言語版の uecsrxdump と同じオプション・同じ出力書式で動作する。

# 起動方法

コマンドラインから以下のように入力する。
その際に、オプションをつけることで希望する情報のみ出力したり、
表示方法を変更することが出来る。

     $ uecsrxdump.py [Options]

または

     $ python3 uecsrxdump.py [Options]

終了するときは Ctrl-C を押す(1秒以内に終了する)。

## オプション

起動時に指定するオプションとはたらきは以下の通り。

|   Option        |     意味                                  |
|:---------------:|:------------------------------------------|
|   -h            | ヘルプの表示                              |
|   -v            | Versionの表示                             |
|   -t            | 日時情報の付記                            |
|   -c CCM名      | 指定したCCMだけを出力する                 |
|   -i IPアドレス | 指定したIPアドレスからのCCMだけを出力する |
|   -m            | 冗長なXMLタグを省略出力 (中省略)          |
|   -s            | 不要なタグを排除してCSV出力 (CSV出力)     |

CCMは、大小文字を区別し全部一致で表示対象となる。
IPアドレスは全部一致で表示対象となる。
IPアドレスはパケットの送信元ではなく、パケット中の `<IP>` タグの値で判定する。

`-m` と `-s` は、UECSのバージョン(ver属性)や属性の並び順に依存せずに処理する。
`-s` では DATA 以外のパケット(NODESCAN など)はCSVに変換できないため出力しない。
`-m` と `-s` を同時に指定した場合は `-s` が優先される。

出力は1行ごとにフラッシュされるため、パイプやファイルへのリダイレクトでも
受信と同時に書き出される。

     $ uecsrxdump.py -t -s >> ccm.log

# 出力書式

## 基本書式

オプションを付けない場合

     $ uecsrxdump.py
     <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="cnd.aXX" room="1" region="2" order="1" priority="1">0</DATA><IP>192.168.0.178</IP></UECS>
     <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="InAirTemp.mIC" room="1" region="3" order="1" priority="29">16.9</DATA><IP>192.168.0.191</IP></UECS>
     <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="InAirHumid.mIC" room="1" region="3" order="1" priority="29">100.0</DATA><IP>192.168.0.191</IP></UECS>
     <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="InAirHumidDef.mIC" room="1" region="3" order="1" priority="29">0.00</DATA><IP>192.168.0.191</IP></UECS>
     <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="CO2.mIC" room="1" region="3" order="1" priority="29">293</DATA><IP>192.168.0.191</IP></UECS>

## -t 日時を付記した場合

     $ uecsrxdump.py -t
     2020-05-27 23:52:48 <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="Relayopr.4.aXX" room="1" region="11" order="4" priority="1">0</DATA><IP>192.168.0.162</IP></UECS>
     2020-05-27 23:52:48 <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="cnd.aXX" room="1" region="10" order="1" priority="1">268435456</DATA><IP>192.168.0.161</IP></UECS>
     2020-05-27 23:52:48 <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="cnd.mCD" room="100" region="1" order="1" priority="29">0</DATA><IP>192.168.0.220</IP></UECS>
     2020-05-27 23:52:48 <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="funcsel.mCD" room="100" region="1" order="1" priority="15">394</DATA><IP>192.168.0.220</IP></UECS>
     2020-05-27 23:52:48 <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="blower.mCD" room="100" region="1" order="1" priority="15">2</DATA><IP>192.168.0.220</IP></UECS>

## -t と中省略の場合

     $ uecsrxdump.py -t -m
     2020-05-27 23:54:34 <DATA type="CndWSS.aXX" room="1" region="1" order="1" priority="5">1</DATA><IP>192.168.0.210</IP>
     2020-05-27 23:54:34 <DATA type="cnd.mCD" room="100" region="1" order="1" priority="29">0</DATA><IP>192.168.0.220</IP>
     2020-05-27 23:54:34 <DATA type="funcsel.mCD" room="100" region="1" order="1" priority="15">393</DATA><IP>192.168.0.220</IP>
     2020-05-27 23:54:34 <DATA type="mode.mCD" room="100" region="1" order="1" priority="15">0</DATA><IP>192.168.0.220</IP>
     2020-05-27 23:54:34 <DATA type="runmode.mCD" room="100" region="1" order="1" priority="15">0</DATA><IP>192.168.0.220</IP>

## -t とCSV出力の場合

CSVの列は `type,room,region,order,priority,値,IPアドレス` の順。
`<IP>` タグがないパケットは、IPアドレスの列が空になる。

     $ uecsrxdump.py -t -s
     2020-05-27 23:57:15 cnd.aXX,1,2,1,1,0,192.168.0.178
     2020-05-27 23:57:15 cnd.aXX,1,11,1,1,268435456,192.168.0.162
     2020-05-27 23:57:15 CndWSS.aXX,1,1,1,5,1,192.168.0.210
     2020-05-27 23:57:15 InAirTemp.mIC,1,3,1,29,16.4,192.168.0.191
     2020-05-27 23:57:15 cnd.cMC,1,1,1,1,0,192.168.0.70

## IPアドレスを特定して出力を得る

     $ uecsrxdump.py -t -m -i 192.168.0.210
     2020-05-27 23:59:33 <DATA type="CndWSS.aXX" room="1" region="1" order="1" priority="5">1</DATA><IP>192.168.0.210</IP>
     2020-05-27 23:59:34 <DATA type="CndWSS.aXX" room="1" region="1" order="1" priority="5">1</DATA><IP>192.168.0.210</IP>
     2020-05-27 23:59:35 <DATA type="CndWSS.aXX" room="1" region="1" order="1" priority="5">1</DATA><IP>192.168.0.210</IP>
     2020-05-27 23:59:36 <DATA type="CndWSS.aXX" room="1" region="1" order="1" priority="5">1</DATA><IP>192.168.0.210</IP>
     2020-05-27 23:59:37 <DATA type="CndWSS.aXX" room="1" region="1" order="1" priority="5">1</DATA><IP>192.168.0.210</IP>

## CCM名を特定して出力を得る

     $ uecsrxdump.py -t -m -c cnd.aXX
     2020-05-28 00:04:29 <DATA type="cnd.aXX" room="1" region="10" order="1" priority="1">268435456</DATA><IP>192.168.0.161</IP>
     2020-05-28 00:04:30 <DATA type="cnd.aXX" room="1" region="2" order="1" priority="1">0</DATA><IP>192.168.0.178</IP>
     2020-05-28 00:04:30 <DATA type="cnd.aXX" room="1" region="11" order="1" priority="1">268435456</DATA><IP>192.168.0.162</IP>
     2020-05-28 00:04:30 <DATA type="cnd.aXX" room="1" region="10" order="1" priority="1">268435456</DATA><IP>192.168.0.161</IP>
     2020-05-28 00:04:31 <DATA type="cnd.aXX" room="1" region="2" order="1" priority="1">0</DATA><IP>192.168.0.178</IP>

## IPアドレスとCCM名を特定して、CSV出力の場合

     $ uecsrxdump.py -t -c cnd.aXX -i 192.168.0.178 -s
     2020-05-28 00:08:28 cnd.aXX,1,2,1,1,0,192.168.0.178
     2020-05-28 00:08:29 cnd.aXX,1,2,1,1,0,192.168.0.178
     2020-05-28 00:08:30 cnd.aXX,1,2,1,1,0,192.168.0.178
     2020-05-28 00:08:31 cnd.aXX,1,2,1,1,0,192.168.0.178
     2020-05-28 00:08:32 cnd.aXX,1,2,1,1,0,192.168.0.178

# How to Install

標準ライブラリだけで動作するため、追加のパッケージは不要。
実行権限を付けて /usr/local/bin の配下にコピーする。

## Requirement

* Python 3.10 以降

型ヒントに `str | None` の書き方を使っているため、Python 3.9 以前では起動できない。
インストール済みのバージョンは以下で確認できる。

     $ python3 --version

## Installing

     $ chmod +x uecsrxdump.py
     $ sudo install uecsrxdump.py /usr/local/bin/

C言語版と同じ `uecsrxdump` の名前で使いたい場合は、拡張子を外してインストールする。
その場合、C言語版の uecsrxdump は上書きされる。

     $ sudo install uecsrxdump.py /usr/local/bin/uecsrxdump

## C言語版との違い

* gcc や make は不要で、Python 3 があればそのまま動く
* `-h` でヘルプを表示できる。未知のオプションを指定するとエラーメッセージを表示して終了する
* `-c` と `-i` の引数に長さの制限はない
* 受信待ちは1秒ごとのタイムアウトで行うため、Ctrl-C を押してから終了まで最大1秒かかる
* 受信データはUTF-8として解釈し、解釈できないバイトは置換文字(�)で表示する
* 起動時間やメモリ使用量はC言語版より大きいが、通常の受信量ではCPU負荷の差はほとんどない

## Author

* 堀本　正文 (HORIMOTO Masafumi)
* YS Lab LLC.
* mh(at)ys-lab.tech

# License

"uecsrxdump" is under [MIT license](https://opensource.org/license/mit)
