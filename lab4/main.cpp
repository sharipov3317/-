#include <bits/stdc++.h>

using namespace std;
// функция случайного выбора параметра
int getRand(int mod)//это Остаток от деления a на b
{
    static int t = 0;
    int tmp = clock(); //это временный файл, автоматически создаваемый программой 
    t = (t + 14) % 81;
    return (1ll * tmp * t) % mod;       
}
 // через эта функция принимает три случайных числа 
void getKey(int mod)
{
    int a, b, m = mod, c0;  
    a = getRand(m);
    a += abs(a % 4 - 1);
    do b = getRand(m); while(__gcd(b, m) != 1 || b % 2 == 0);
    c0 = getRand(m);
  // записываем полученные числа в key
    ofstream key("key");
    key << a << " " << b << " " << c0;
    key.close();
}
 
int main()
{
  // устанавливаем русский язык
    setlocale(LC_ALL, "");
    cout << "Введите key для генерации ключа\n"
         << "Введите crypt <входной файл> <выходной файл> чтобы зашифровать файл" <<
            endl;
    string str;
    cin >> str;
    int mod = (1 << 8); 
  // если введено key
    if(str == "key")
    {
      // получаем новый ключ
        getKey(mod);
        cout << "Ключ готов" << endl;
    }
      // если введено crypt
    else if(str == "crypt")
    {       
        string file, outfile;
        cin >> file >> outfile;
      // открываем файлы
        FILE* f = fopen(file.c_str(), "r+b");
        FILE* of = fopen(outfile.c_str(), "w+b");
      // читаем ключ
        ifstream key("key");
        int a, b, c0, m = mod;
        key >> a >> b >> c0;
        char buf[1];   
      // фиксируем время начала шифрования
        auto beg = clock();
      // шифруем каждый байт
        while(fread(buf, 1, sizeof(char), f) == 1)
        {
          // гаммирование
            buf[0] ^= c0;
          // записываем зашифрованный байт в выходной файл
            if(fwrite(buf, 1, sizeof(char), of) != 1)
            {
                cout << "Ошибка!" << endl;
                return 1;
            }
          // получаем новое случайное число
            c0 = (1ll * c0 * a + b) % m;            
        }
        cout << buf[0] << endl;
      // конец измерения времени
        auto en = clock();
      cout << "Время шифрования:" << endl;
        cout << float(en - beg) / CLOCKS_PER_SEC << endl;
        fclose(f);
        fclose(of);
        key.close();
    }
}