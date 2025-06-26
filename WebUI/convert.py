import gzip
import argparse
import re


import gzip
import argparse
import re
import os


def html_to_uint8_array(html_path, output_array_path):
    with open(html_path, "rb") as f:
        html_data = f.read()

    gzipped_data = gzip.compress(html_data)
    array_name = os.path.splitext(os.path.basename(output_array_path))[0]
    with open(output_array_path, "w") as f:
        f.write(f"const uint8_t {array_name}[{len(gzipped_data)}] PROGMEM = {{\n  ")
        for i, b in enumerate(gzipped_data):
            f.write(str(b))
            if i < len(gzipped_data) - 1:
                f.write(", ")
            if (i + 1) % 20 == 0:
                f.write("\n  ")
        f.write("\n};\n")

    print(f"✅ Đã chuyển '{html_path}' → mảng uint8_t[] trong '{output_array_path}' với tên mảng '{array_name}'")



def uint8_array_to_html(array_path, output_html_path):
    with open(array_path, "r") as f:
        content = f.read()

    # Tìm tất cả các số trong mảng
    numbers = re.findall(r"\b\d+\b", content)
    byte_data = bytearray(int(num) for num in numbers)

    # Giải nén GZIP
    html_data = gzip.decompress(byte_data)

    with open(output_html_path, "wb") as f:
        f.write(html_data)

    print(f"✅ Đã chuyển '{array_path}' → HTML tại '{output_html_path}'")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Chuyển HTML ↔ uint8_t GZIP array")
    parser.add_argument("mode", choices=["to_array", "to_html"], help="Chế độ: to_array hoặc to_html")
    parser.add_argument("input", help="File đầu vào (HTML hoặc array)")
    parser.add_argument("output", help="File đầu ra (array hoặc HTML)")

    args = parser.parse_args()

    if args.mode == "to_array":
        html_to_uint8_array(args.input, args.output)
    elif args.mode == "to_html":
        uint8_array_to_html(args.input, args.output)
