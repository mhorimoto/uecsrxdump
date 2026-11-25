#!/usr/bin/env python3

import argparse
import re
import signal
import socket
import sys
from datetime import datetime

VERSION = "1.20"
PORT = 16520
BUFSIZE = 2048


def ccm_strip(data: str) -> str:
    idx = data.find("<UECS")
    if idx == -1:
        return data
    start = data.find(">", idx)
    if start == -1:
        return data
    start += 1
    end = data.find("</UECS>", start)
    if end == -1:
        end = len(data)
    return data[start:end]


def ccm_to_csv(data: str) -> str | None:
    m = re.search(r'<DATA(\s[^>]*)>(.*?)</DATA>', data, re.DOTALL)
    if not m:
        return None
    attrs = m.group(1)
    value = m.group(2)

    def get_attr(name: str) -> str:
        am = re.search(r'(?<=\s)' + re.escape(name) + r'="([^"]*)"', attrs)
        return am.group(1) if am else ""

    type_ = get_attr("type")
    if not type_:
        return None

    ip_m = re.search(r'<IP>(.*?)</IP>', data)
    ipaddr = ip_m.group(1) if ip_m else ""

    return ",".join([
        type_,
        get_attr("room"),
        get_attr("region"),
        get_attr("order"),
        get_attr("priority"),
        value,
        ipaddr,
    ])


def main() -> None:
    parser = argparse.ArgumentParser(
        prog="uecsrxdump",
        description="UECS CCM受電データダンプ",
    )
    parser.add_argument("-v", action="store_true", help="Versionの表示")
    parser.add_argument("-t", action="store_true", help="日時情報の付記")
    parser.add_argument("-c", metavar="CCM名", help="指定したCCMだけを出力する")
    parser.add_argument("-i", metavar="IPアドレス", help="指定したIPアドレスからのCCMだけを出力する")
    parser.add_argument("-m", action="store_true", help="冗長なXMLタグを省略出力")
    parser.add_argument("-s", action="store_true", help="不要なタグを排除してCSV出力")
    args = parser.parse_args()

    if args.v:
        print(f"uecsrxdump UECS CCM packet Dump Version {VERSION}")
        sys.exit(0)

    ip_filter = f"<IP>{args.i}</IP>" if args.i else None
    ccm_filter = f'type="{args.c}"' if args.c else None

    running = True

    def handle_sigint(sig, frame):
        nonlocal running
        running = False

    signal.signal(signal.SIGINT, handle_sigint)

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind(("", PORT))
        sock.settimeout(1.0)

        while running:
            try:
                data, _ = sock.recvfrom(BUFSIZE)
            except socket.timeout:
                continue
            except OSError:
                break

            pkt = data.decode("utf-8", errors="replace").rstrip("\x00")

            if ip_filter and ip_filter not in pkt:
                continue
            if ccm_filter and ccm_filter not in pkt:
                continue

            if args.s:
                result = ccm_to_csv(pkt)
                if result is None:
                    continue
            elif args.m:
                result = ccm_strip(pkt)
            else:
                result = pkt

            if args.t:
                ts = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                print(f"{ts} {result}")
            else:
                print(result)
            sys.stdout.flush()


if __name__ == "__main__":
    main()
