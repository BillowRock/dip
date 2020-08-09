# dip —— Software for **Digital Image Processing** using Qt

## 1. Environment Configuration
windows 10 + Qt 5.14.2 + MSVC 2017 + fftw 64bit

## 2. Function Lists

### 1) MDI mode
### 2) View menu: grayscale, spectrum, histogram, negative, pseudo color
### 3) Enhancement menu: Histogram equalization, Adaptive contrast enhancement
### 4) Filter menu: Space domain, Frequency domain, Emboss filter
### 5) Multiple language dynamic switching
### 6) EdgeDetectation: Sobel opeartor, Roberts opeartor

## 3. Fix

### 1) Change the the three buttons in the dialog when the modified picture is closed and add Chinese translation.
### 2) The image size in filter dialog is limited to prevent the widget from being invisible due to large pictures
### 3) Change the first emboss_kernel
### 4) Add some comments(mainly in imageprocess)
### 5) Add EdgeDetectation Function
