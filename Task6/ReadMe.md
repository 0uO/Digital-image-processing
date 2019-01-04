---
style: plain
---
6th Task: Image segmentation
===========

[[toc]]

## Pic.h updated

- `Class Picture`
	removed.

+ `Class metaPicture`

	+ metaPicture();
	+ unsigned int getXlen()const;
	+ unsigned int getYlen()const;
	+ unsigned short getBandNum()const;
	+ Pixel ***getPic()const;
	+ Pixel getPixel(const int x, const int y)const;
	+ bool update();
	+ ~metaPicture();

+ `Class Picture: public metaPicture`

	+ Picture(const char *_path = nullptr);
	+ Picture(const unsigned int x, const unsigned int y, const unsigned short b = 3);
	+ bool save(const char* dstPath, const char* format="JPEG");
	+ bool Cov(Kernel *k);
	+ ~Picture();

+ `Class LargePicture: public metaPicture`

	+ LargePicture(const char *_path, const unsigned short B_X = 0, const unsigned short B_Y = 256,const char *dst_path = "\0");
	+ LargePicture(const unsigned int x, const unsigned int y, const unsigned short b = 3, const unsigned short B_X = 0, const unsigned short B_Y = 256, const char *dst_path = "\0");

		+ The parameter B_X, B_Y indicate the size of block.

	+ unsigned short getBlockXlen()const;
	+ unsigned short getBlockYlen()const;
	+ unsigned int getXpointer()const;
	+ unsigned int getYpointer()const;

	+ bool nextPointer();
	+ unsigned short nextLoadSizeX();
	+ unsigned short nextLoadSizeY();
	+ bool next();

		+ Move to the next block.

	+ void reset();
	+ bool update();
	+ ~LargePicture();

Then I move all the implementations of function into Pic.cpp. Check the file for more details. 

## IHS

I rewrite it and package it as a function in Pic.cpp, make the function more clear.

+ LargePicture IHS(LargePicture* P_MUL, LargePicture* P_PAN, const char* dst_file = "ans.tif");

Which return a new pointer point to the large picture,  remember deleting it when used done.

### Result

The picture is to large to push. Here is the example, using last experiment's material.

![ans_example](ans_example%202.jpg)

#### Time
	
	image size: 15696 X 12004, 3-channal

	using block with size image_width*256:  501.702s
	using block with size 256*image_length:  2474.712s

	Note that using block with size 256*image_length is too slow, so I just estimate it with top 2048*image_length pixel with costing time 309.339s.

## Some problem

+ Picture generated in IHS is usually too large that I can't load whole of it into the memory, so that I can't  transfer it into .jpg.

+ There are some weird Pixel which should be white but pink or yellow, I can't figure the reason.






