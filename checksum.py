import sys

def ip_checksum(data: bytes) -> int:
    if len(data) % 2 == 1:
        data += b'\x00'  # pad to even length

    checksum = 0
    for i in range(0, len(data), 2):
        word = int.from_bytes(data[i:i+2], signed=False, byteorder="little")
        #word = (data[i]) + (data[i + 1] << 8)  # little-endian
        checksum += word
        checksum = (checksum & 0xFFFF) + (checksum >> 16)  # fold overflow

    return ~checksum & 0xFFFF  # one's complement


# Example usage:
def main():
    blk_num = 0
    if len(sys.argv) > 1:
        try:
            blk_num = int(sys.argv[1])
        except:
            print(f"usage: python {sys.argv[0]} <blk_num>")
            sys.exit()
    
    
    with open("./build/os-x86_64.img", "rb") as f:
        data = f.read(512*(blk_num+1))
    data = data[512*blk_num:512*(blk_num+1)]

    checksum = ip_checksum(data)
    print(f"Checksum for block {blk_num}: 0x{checksum:04X}")


if __name__ == "__main__":
    main()

