# Questions

## What's `stdint.h`?

It is a header file

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

portability

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

 `BYTE` - 1
 `DWORD` - 4
 `LONG` - 4
 `WORD` - 2

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

The file type; must be BM

## What's the difference between `bfSize` and `biSize`?

bfSize = The size, in bytes, of the bitmap file
biSize = The number of bytes required by the structure.

## What does it mean if `biHeight` is negative?

the bitmap is a top-down DIB and its origin is the upper-left corner

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount

## Why might `fopen` return `NULL` in `copy.c`?

'NULL' is returned if there is any error during 'fopen'

## Why is the third argument to `fread` always `1` in our code?

1 item

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

 padding = (4 - (3 * sizeof(RGBTRIPLE)) % 4) % 4 = 0

## What does `fseek` do?

It sets the file position indicator for the stream, in order to jump over the padding

## What is `SEEK_CUR`?

the current file position indicator
