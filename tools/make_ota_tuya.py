#!/usr/bin/env python3

import argparse
import binascii
import struct

OTA_MAGIC = b'\x5d\x02'


def append_crc(firmware):
    # Ensure FW size is multiple of 16
    padding = 16 - len(firmware) % 16
    if padding < 16:
        firmware += b'\xFF' * padding

    # Add CRC
    crc = binascii.crc32(firmware) ^ 0xffffffff
    firmware += crc.to_bytes(4, byteorder='little')

def patch_firmware(firmware):
    if firmware[6:8] != OTA_MAGIC:
        append_crc(firmware)
        # Fix FW length
        firmware[0x18:0x1c] = (len(firmware)).to_bytes(4, byteorder='little')
        # Add magic constant
        firmware[6:8] = OTA_MAGIC


def get_ota_header(firmware, args):
    ota_hdr_s = struct.Struct('<I5HIH32sI')
    header_size = 56
    firmware_len = len(firmware)
    total_image_size = firmware_len + header_size + 6
    manufacturer_code = args.set_manufacturer or int.from_bytes(firmware[18:20], byteorder='little')
    image_type = args.set_image_type or int.from_bytes(firmware[20:22], byteorder='little')
    file_version = args.set_version or int.from_bytes(firmware[2:6], byteorder='little')
    ota_hdr = ota_hdr_s.pack(
        0xbeef11e,
        0x100,  # header version is 0x0100
        header_size,
        0,  # ota_ext_hdr_value if ota_ext_hdr else 0,
        manufacturer_code,  # args.manufacturer,
        image_type,  # args.image_type,
        file_version,  # options.File_Version
        args.ota_version,  # options.stack_version,
        b'\x00' * 32,  # OTA_Header_String.encode(),
        total_image_size,
    )
    # add chunk header: 0 - firmware type
    ota_hdr += struct.pack('<HI', 0, firmware_len)
    return ota_hdr


def main(args):
    with open(args.output, 'wb') as out:
        with open(args.firmware, 'rb') as firm:
            firmware = bytearray(firm.read(-1))
        patch_firmware(firmware)
        outbytes = firmware

        with open(args.appendix, 'rb') as app:
            appendix = app.read(-1)
        outbytes += appendix
        append_crc(outbytes)
        out.write(get_ota_header(outbytes, args))
        out.write(outbytes)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("firmware", help="path to firmware")
    parser.add_argument("appendix", help="path to file to append")
    parser.add_argument("-o", '--output', help="path to output file", required=True)
    parser.add_argument("-m", '--set-manufacturer', type=lambda x: int(x, 0), help="Override manufacturer code")
    parser.add_argument("-t", '--set-image-type', type=lambda x: int(x, 0), help="Override image type")
    parser.add_argument("-v", '--set-version', type=lambda x: int(x, 0), help="Override version")
    parser.add_argument("-s", '--ota-version', type=int, help="OTA stack version", default=2)

    _args = parser.parse_args()
    main(_args)
