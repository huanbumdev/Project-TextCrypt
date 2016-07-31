#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include "TextCrypt.h"
#include "SHA512.h"
#define Encrypting true
#define Decrypting false
using namespace std;

int main(){
    int Option;
    string FileName;
    cout << "Option 1: Encrypt text." << endl;
    cout << "Option 2: Decrypt text." << endl;
    cout << "Instruction: Hit Enter twice to stop typing." << endl;
    cout << "Option 3: Encrypt file." << endl;
    cout << "Option 4: Decrypt file." << endl;
    cout << "Your option: ";
    do{
        fflush(stdin);
        cin >> Option;
        if (Option != 1 && Option != 2 && Option != 3 && Option != 4){
            cout << "Retry: ";
        }
    } while (Option != 1 && Option != 2 && Option != 3 && Option != 4);
    if (Option == 1){
        TextCrypt Text(Encrypting);
    }
    else if (Option == 2){
        TextCrypt Text(Decrypting);
    }
    else if (Option == 3){
        cout << "File: ";
        do {
            fflush(stdin);
            getline(cin, FileName);
            if (FileName.empty()){
                cout << "Retry: ";
            }
        } while (FileName.empty());
        TextCrypt(Encrypting, FileName);
    }
    else if (Option == 4){
        cout << "File: ";
        do {
            fflush(stdin);
            getline(cin, FileName);
            if (FileName.empty()){
                cout << "Retry: ";
            }
        } while (FileName.empty());
        TextCrypt(Decrypting, FileName);
    }
    system("pause");
    return 0;
}
