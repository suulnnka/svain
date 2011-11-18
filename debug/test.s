push x
pop 栈顶元素
add sub mul div mod 操作两个栈顶元素
store 存堆
retrieve 取栈
sign x 做标记
jmp x 跳到sign x处
je x 如果栈顶为0,跳到sign x处
jne x 如果栈顶不为0,跳到sign x处
call x 呼叫sign x处的子程序 push pc.push 栈顶.栈顶 = 栈底.
ret 返回子程序
readnum,readchar 输入数字或字符
outnum,outchar 输出数字或字符

cp
sli
dup
swap