mcro my_mcro
x: .data -5 ,4,3,2,1
str: .string   "Hello World" 
mcroend
LABELI: .mat [1][3] 1,  4,     6
my_mcro 
.extern LENGTH
   mov   #+3,  aaa[r1][r3]
; FFF
   clear:   clr   x
stop
