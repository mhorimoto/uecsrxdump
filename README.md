# uecsrxdump

UECS用CCM受電データダンププログラム

UECSの16520/udpのパケットを受電して整形して標準出力に出力する。

# 起動方法

コマンドラインから以下のように入力する。
その際に、オプションをつけることで希望する情報のみ出力したり、
表示方法を変更することが出来る。

     $ uecsrxdump [Options]

## オプション

起動時に指定するオプションとはたらきは以下の通り。

|   Option        |     意味                                  |
|:---------------:|:------------------------------------------|
|   -v            | Versionの表示                             |
|   -t            | 日時情報の付記                            |
|   -c CCM名      | 指定したCCMだけを出力する                 |
|   -i IPアドレス | 指定したIPアドレスからのCCMだけを出力する |
|   -m            | 冗長なXMLタグを省略出力 (中省略)          |
|   -s            | 不要なタグを排除してCSV出力 (CSV出力)     |

CCMは、大小文字を区別し全部一致で表示対象となる。
IPアドレスは全部一致で表示対象となる。

# 出力書式

## 基本書式

オプションを付けない場合

     $ uecsrxdump
     <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="cnd.aXX" room="1" region="2" order="1" priority="1">0</DATA><IP>192.168.0.178</IP></UECS>
     <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="InAirTemp.mIC" room="1" region="3" order="1" priority="29">16.9</DATA><IP>192.168.0.191</IP></UECS>
     <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="InAirHumid.mIC" room="1" region="3" order="1" priority="29">100.0</DATA><IP>192.168.0.191</IP></UECS>
     <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="InAirHumidDef.mIC" room="1" region="3" order="1" priority="29">0.00</DATA><IP>192.168.0.191</IP></UECS>
     <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="CO2.mIC" room="1" region="3" order="1" priority="29">293</DATA><IP>192.168.0.191</IP></UECS>

## -t 日時を付記した場合

     $ uecsrxdump -t
     2020-05-27 23:52:48 <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="Relayopr.4.aXX" room="1" region="11" order="4" priority="1">0</DATA><IP>192.168.0.162</IP></UECS>
     2020-05-27 23:52:48 <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="cnd.aXX" room="1" region="10" order="1" priority="1">268435456</DATA><IP>192.168.0.161</IP></UECS>
     2020-05-27 23:52:48 <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="cnd.mCD" room="100" region="1" order="1" priority="29">0</DATA><IP>192.168.0.220</IP></UECS>
     2020-05-27 23:52:48 <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="funcsel.mCD" room="100" region="1" order="1" priority="15">394</DATA><IP>192.168.0.220</IP></UECS>
     2020-05-27 23:52:48 <?xml version="1.0"?><UECS ver="1.00-E10"><DATA type="blower.mCD" room="100" region="1" order="1" priority="15">2</DATA><IP>192.168.0.220</IP></UECS>

## -t と中省略の場合

     $ uecsrxdump -t -m
     2020-05-27 23:54:34 <DATA type="CndWSS.aXX" room="1" region="1" order="1" priority="5">1</DATA><IP>192.168.0.210</IP>
     2020-05-27 23:54:34 <DATA type="cnd.mCD" room="100" region="1" order="1" priority="29">0</DATA><IP>192.168.0.220</IP>
     2020-05-27 23:54:34 <DATA type="funcsel.mCD" room="100" region="1" order="1" priority="15">393</DATA><IP>192.168.0.220</IP>
     2020-05-27 23:54:34 <DATA type="mode.mCD" room="100" region="1" order="1" priority="15">0</DATA><IP>192.168.0.220</IP>
     2020-05-27 23:54:34 <DATA type="runmode.mCD" room="100" region="1" order="1" priority="15">0</DATA><IP>192.168.0.220</IP>

## -t とCSV出力の場合

     $ uecsrxdump -t -s
     2020-05-27 23:57:15 cnd.aXX,1,2,1,1,0,192.168.0.178
     2020-05-27 23:57:15 cnd.aXX,1,11,1,1,268435456,192.168.0.162
     2020-05-27 23:57:15 CndWSS.aXX,1,1,1,5,1,192.168.0.210
     2020-05-27 23:57:15 InAirTemp.mIC,1,3,1,29,16.4,192.168.0.191
     2020-05-27 23:57:15 cnd.cMC,1,1,1,1,0,192.168.0.70

## IPアドレスを特定して出力を得る

     $ uecsrxdump -t -m -i 192.168.0.210
     2020-05-27 23:59:33 <DATA type="CndWSS.aXX" room="1" region="1" order="1" priority="5">1</DATA><IP>192.168.0.210</IP>
     2020-05-27 23:59:34 <DATA type="CndWSS.aXX" room="1" region="1" order="1" priority="5">1</DATA><IP>192.168.0.210</IP>
     2020-05-27 23:59:35 <DATA type="CndWSS.aXX" room="1" region="1" order="1" priority="5">1</DATA><IP>192.168.0.210</IP>
     2020-05-27 23:59:36 <DATA type="CndWSS.aXX" room="1" region="1" order="1" priority="5">1</DATA><IP>192.168.0.210</IP>
     2020-05-27 23:59:37 <DATA type="CndWSS.aXX" room="1" region="1" order="1" priority="5">1</DATA><IP>192.168.0.210</IP>

## CCM名を特定して出力を得る

     $ uecsrxdump -t -m -c cnd.aXX
     2020-05-28 00:04:29 <DATA type="cnd.aXX" room="1" region="10" order="1" priority="1">268435456</DATA><IP>192.168.0.161</IP>
     2020-05-28 00:04:30 <DATA type="cnd.aXX" room="1" region="2" order="1" priority="1">0</DATA><IP>192.168.0.178</IP>
     2020-05-28 00:04:30 <DATA type="cnd.aXX" room="1" region="11" order="1" priority="1">268435456</DATA><IP>192.168.0.162</IP>
     2020-05-28 00:04:30 <DATA type="cnd.aXX" room="1" region="10" order="1" priority="1">268435456</DATA><IP>192.168.0.161</IP>
     2020-05-28 00:04:31 <DATA type="cnd.aXX" room="1" region="2" order="1" priority="1">0</DATA><IP>192.168.0.178</IP>

## IPアドレスとCCM名を特定して、CSV出力の場合

     $ uecsrxdump -t -c cnd.aXX -i 192.168.0.178 -s
     2020-05-28 00:08:28 cnd.aXX,1,2,1,1,0,192.168.0.178
     2020-05-28 00:08:29 cnd.aXX,1,2,1,1,0,192.168.0.178
     2020-05-28 00:08:30 cnd.aXX,1,2,1,1,0,192.168.0.178
     2020-05-28 00:08:31 cnd.aXX,1,2,1,1,0,192.168.0.178
     2020-05-28 00:08:32 cnd.aXX,1,2,1,1,0,192.168.0.178

# How to Install

uecsrxdumpプログラムは、 /usr/local/bin の配下にインストールされる。

## Requirement

* gcc
* make

## Making

     $ make
     $ sudo make install

## Author

* 堀本　正文 (HORIMOTO Masafumi)
* HOLLY&amp;Co.,Ltd.
* horimoto@holly-linux.com

# License

"uecsrxdump" is under [MIT license](https://ja.osdn.net/projects/opensource/wiki/licenses%2FMIT_license)
