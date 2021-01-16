#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define NUM_LETTERS ((int)26)

typedef enum {FALSE=0, TRUE=1} boolean;

typedef struct node{
    char letter;
    long unsigned int count; // number of words end here
    struct node* children[NUM_LETTERS];
    boolean isEndOfWord; // is end of word
} node;

struct node* getNewNode(){
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->isEndOfWord = 0;
    for (int i = 0; i < NUM_LETTERS; ++i) {
        temp->children[i] = NULL;
    }
    return temp;
}

int insert(node *root, char *word) {
    int counter= 0;
    if (word==NULL || strlen(word) == 0) return counter;
    for(int i = 0; i < strlen(word); i++){
        if(isalpha(word[i])){
             counter++;
        }
    }
    if (counter==0) return counter;

    char PWord[counter];
    int j = 0;
    for (int i = 0; strlen(word) > i; i++) {
        char letter = word[i];
        if(isalpha(letter)){
            letter = tolower(letter);
            PWord[j] = letter;
            j++;
        }
    }

    struct node* curr = root;
    for(int i=0; i < counter; i++) {
        if(curr->children[PWord[i]- 'a'] == NULL){
            node *temp = getNewNode();
            temp->letter = PWord[i];
            temp->count = 0;
            if(i == counter - 1) {
                temp->isEndOfWord = TRUE;
                temp->count = 1;
            }

            curr->children[PWord[i]-'a'] = temp;
            curr = temp;
        }
        else  {
            if(i == counter - 1) {
                curr->children[PWord[i]- 'a']->isEndOfWord =1;
                curr->children[PWord[i]- 'a']->count = curr->children[PWord[i]- 'a']->count+ 1;
            }
            curr = curr->children[PWord[i]- 'a'];
        }
    }
    return counter;
}

int buildTrie(node *root, char *input) {
    int size = 0;
    if (input == NULL) return size;
    const char s[4] = " \n\t\r";
    char *arr = strtok(input, s);
    while (arr != NULL) {
        if(arr && arr!=(void *)0 ) {
            size += insert(root, arr);
            arr = strtok(NULL, s);
        }
    }
    return size;
}

void reverse(struct node *follow, char *hold, int s){
    int i;
    if(follow == NULL){
        return;
    }

    for(i = NUM_LETTERS - 1; i >= 0; i--){
        hold[s] = 'a'+i;
        reverse(follow->children[i], hold, s + 1);
    }

    if (follow->isEndOfWord) {
        hold[s] = 0;
        printf("%s %lu\n", hold, follow->count);
    }
}


void preorder(struct node *follow, char *hold, int s){
    int i;
    if(follow == NULL){
        return;
    }
    if (follow->isEndOfWord) {
        hold[s] = 0;
        printf("%s %lu\n", hold, follow->count);
    }

    for(i = 0; i < NUM_LETTERS; i++){
        hold[s] = 'a'+i;
        preorder(follow->children[i], hold, s + 1);
    }
}

void free_trienode(struct node* node) {
    for(int i=0; i<NUM_LETTERS; i++) {
        if (node->children[i] != NULL) {
            free_trienode(node->children[i]);
        }
        else {
            continue;
        }
    }
    free(node);
}


int main(int argc, char **argv) {

    node *root = getNewNode();
    int s= 500;
    char *txt = (char*)calloc(s,sizeof(char));
    if (!txt) {
        printf("Malloc failed\n");
        return -1;
    }
    int i=0;
    int temp;
    while((temp = fgetc(stdin)) != -1){
        if (i >= s){
            s = s*2;
            txt = (char*)realloc(txt,s*sizeof(char));
            if (!txt){
                printf("Realloc failed\n");
            }
        }
        *(txt+i)= temp;
        i++;
    }
    txt[i]='\0';

    int size = buildTrie(root, txt);
    char hold[size];
    if (argc == 2) {
        if (*argv[1] == 'r') {
            reverse(root, hold, 0);
        }
    } else {
        preorder(root, hold, 0);
    }

    free_trienode(root);
    free(txt);
    return 0;
}