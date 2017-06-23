
echo -n "json:      " && cd json && gcc main.c json.c -lcleri && ./a.out
echo -n "choice:    " && cd ../choice && gcc main.c -lcleri && ./a.out
echo -n "keyword:   " && cd ../keyword && gcc main.c -lcleri && ./a.out
echo -n "list:      " && cd ../list && gcc main.c -lcleri && ./a.out
echo -n "optional:  " && cd ../optional && gcc main.c -lcleri && ./a.out
echo -n "prio:      " && cd ../prio && gcc main.c -lcleri && ./a.out
echo -n "ref:       " && cd ../ref && gcc main.c -lcleri && ./a.out
echo -n "repeat:    " && cd ../repeat && gcc main.c -lcleri && ./a.out
echo -n "sequence:  " && cd ../sequence && gcc main.c -lcleri && ./a.out
echo -n "token:     " && cd ../token && gcc main.c -lcleri && ./a.out
echo -n "tokens:    " && cd ../tokens && gcc main.c -lcleri && ./a.out
