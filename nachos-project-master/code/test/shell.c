#include "syscall.h"

int main() {
    SpaceId newProc;
    OpenFileId input = _ConsoleInput;
    OpenFileId output = _ConsoleOutput;
    char prompt[2], ch, buffer[60];
    
    /* DECLARE ALL VARIABLES HERE (C89 Requirement) */
    char *left, *right;
    int i, j, pipeIndex;
    OpenFileId tempFileId;

    prompt[0] = '-';
    prompt[1] = '-';

    while (1) {
        Write(prompt, 2, output);

        i = 0;
        do {
            Read(&buffer[i], 1, input);
        } while (buffer[i++] != '\n');

        buffer[--i] = '\0';

        /* Initialize pipeIndex here, do not declare it */
        pipeIndex = -1;
        
        /* Use 'j' which was declared at the top */
        for (j = 0; j < i; j++) {
            if (buffer[j] == '|') {
                pipeIndex = j;
                break;
            }
        }

        if (pipeIndex != -1) {
            buffer[pipeIndex] = '\0';
            
            /* Assign values to pointers declared at top */
            left = buffer;
            right = &buffer[pipeIndex + 1];

            if (*right == ' ') right++;

            Create("tmpFile");
            tempFileId = Open("tmpFile", 0);

            if (tempFileId != -1) {
                /* 1. EXECUTE LEFT (Output to File) */
                SetRedirect(input, tempFileId);
                
                newProc = Exec(left);
                Join(newProc);

                /* Reset */
                SetRedirect(input, output);

                /* 2. EXECUTE RIGHT (Input from File) */
                Seek(0, tempFileId);
                SetRedirect(tempFileId, output);

                newProc = Exec(right);
                Join(newProc);

                /* 3. CLEANUP */
                SetRedirect(input, output);
                Close(tempFileId);
            }
        }
        else {
            if (i > 0) {
                newProc = Exec(buffer);
                Join(newProc);
            }
        }
    }
}
