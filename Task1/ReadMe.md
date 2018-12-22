---
style: plain
---
1st Task: GDAL + Visual Studio
===========

[[toc]]

## Install VC 2010 express and GDAL

[VC 2010 express](https://ouceducn-my.sharepoint.com/:u:/g/personal/gaofeng_ouc_edu_cn/EeLSAzv5H5tIgXPm3bADAJcBklbVYqMXw-G6G39t4ZGmEg?e=PZSmoS)

[gdal](https://ouceducn-my.sharepoint.com/:u:/g/personal/gaofeng_ouc_edu_cn/ES8fvuffgxdOs7lSwpTzocYBd4gJFrDde6vNIXFhQyMWeQ?e=7LbJgt)

Offered by my [professor](https://github.com/summitgao), DO NOT FORWORD WITHOUT PERMISSION.

## Save as *.tif

Just use GDAL load the picture into buff area, then write to another .tif file.

![Source image](trees.jpg)

And [result image](trees.tif)

* * *

## Some problem

I try to use gcc to compile the main.cpp but fail, It seems must use VC 2010 as compiler.

Create() function do not  support JPEG、PNG. So I don't know how to deal with .jpeg file by GDAL, and I try to modify the suffix of output .tif file  to .jpg manually, however it cannot diaplay normally in markdown.
