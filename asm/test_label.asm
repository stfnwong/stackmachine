; Test parsing sections with labels


INLINE_LABEL: STORE $10     ; label on same line

; label above - line_num should be the number of the line below (7)
PRE_LINE_LABEL:
    DUP
    ADD 
    SWAP

; Label above, but next character starts at col 0
PRE_LINE_LABEL_NO_INDENT:
XOR

; Label inline, but no colon after
LINE_LABEL_NO_COLON  RPUSH
