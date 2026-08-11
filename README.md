# Fastbit Embedded MCU1

I've made this repository to show the code I have written whilst following this course.

I've made some additions, one of them being adding a private function (RCC_GetPLLOutputClock()) to my RCC driver, allowing me to use the RCC API's with PLL selected as the main clock source.

I've added comments for each API in all of my driver files and board support package files describing what the function does, it's
parameters, and return values.

This driver isn't fully complete as some parts haven't been covered in this course (for eg. Clear to Send (CTS) hardware flow control for USART).
