#include <bits/stdc++.h>

using namespace std;
int arr[102];
/* функция случайного выбора параметра для ГПСЧ */
int getRand(int mod)//это Остаток от деления a на b 
{
  static int t = 0;
  int tmp = clock();
  t = (t + 14) % 81;
  return (1ll * tmp * t) % mod;
}

int main() {
  setlocale(LC_ALL, ""); // Устанавливаем русский язык
  cout << "введите [get], чтобы получить ГПСЧ, или [gen+num], чтобы "
          "сгенерировать определенное количество чисел и записать в файл:"
       << endl;
  string str;
  int a, b, m = (1 << 24), c0; // m равна 10^24
  int interval = (m + 99) / 100;
  // Читаем строку
  while (getline(cin, str)) {
    cout << ">" << endl;

    a = getRand(m);
    a += abs(a % 4 - 1); // а % 4 должно быть равно 1
    do
      b = getRand(m);
    while (__gcd(b, m) != 1 ||
           b % 2 == 0); // b должно быть взаимно простым с m и нечетным
    c0 = getRand(m); // получаем первое число последовательности

    if (str == "get") {
      string tmp;
      do {
        cout << c0 << endl;
        cout << "+, продолжать" << endl;
        cin >> tmp;
        // вычисляем следующее случайное число
        c0 = (1ll * c0 * a + b) % m;
      } while (tmp == "+");
    } else if (str.substr(0, 3) == "gen") {
      // открываем для записи
      ofstream f("file");
      // получаем число после gen
      int num = atoi((str.substr(4, str.size() - 4)).c_str());
      // заполняем массив нулями
      fill(arr, arr + 102, 0);
      for (int i = 0; i < num; i++) {
        // записываем число в файл
        f << c0 << " ";
        // увеличиваем нужный интервал
        arr[c0 / interval]++;
        // вычисляем следующее случайное число
        c0 = (1ll * a * c0 + b) % m;
      }
      double ans = 0;
      double mn = 1;
      double mx = 0;
      for (int i = 0; i < 100; i++) {
        mn = min(mn, 1.0 * arr[i] / num);
        mx = max(mx, 1.0 * arr[i] / num);
        ans += 1.0 * arr[i] / num;
      }
      ans /= 100;
      cout << "Среднее арифметическое относительного частоты - это " << ans
           << endl;
      cout << "Минимум - " << mn << endl;
      cout << "Максимум - " << mx << endl;
    }
  }
}
