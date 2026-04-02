-e _c_int00_secure

MEMORY
{
    TEXT  (RWX):    org = 0xA8400000,   len = 0x00000100
}

SECTIONS
{
    .text:_c_int00_secure > TEXT    ALIGN(0x200000)
}
