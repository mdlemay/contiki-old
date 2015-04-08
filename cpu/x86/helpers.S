.text

.global halt
halt:
        cli
die:    hlt
        jmp die

.global outb
outb:
        mov 4(%esp), %dx
        mov 8(%esp), %al
        out %al, %dx
        ret
