find . -type d \( -path ./.git -o \
                  -path ./log -o \
                  -path ./public -o \
                  -path ./tmp \) -prune -o \
       ! -type d -print > list.txt
