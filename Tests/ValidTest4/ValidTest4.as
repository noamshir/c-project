; this is a comment;
mcro shut_down
jmp myMat1[r1][r3]
add r2,STR
mov r1,r3
stop
mcroend
sub r1,r5
shut_down
myMat1: .mat [2][2] 1,2,3,4
STR: .string "my valid test"