#include <bits/stdc++.h>
using namespace std;
int main(){
    // FILE *fptr;
    // fptr = fopen("example.txt","r");
    // char str[100];
    // while(fgets(str,100,fptr)){
    //      printf("%s\n",str);
    // }
    // fclose(fptr);
    ofstream clientMessageFile;
    clientMessageFile.open(clientMessageFileName,fstream::app);
    char[200] buf = "Geats";
    clientMessageFile<<buf;
    return 0;
}