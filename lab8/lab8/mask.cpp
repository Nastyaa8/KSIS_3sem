#include <iostream>
using namespace std;

// Функция для проверки правильности введенного IP-адреса
bool CheckAddress(char* ip_) {
    int points = 0, // количество точек
        numbers = 0; // значение октета

    char* buff; // буфер для одного октета
    buff = new char[3]; // выделяем память для буфера (макс. 3 цифры)

    // Проход по каждому символу IP-адреса
    for (int i = 0; ip_[i] != '\0'; i++) {
        if (ip_[i] <= '9' && ip_[i] >= '0') { // если цифра
            if (numbers > 3) return false; // если больше трех чисел в октете – ошибка
            buff[numbers++] = ip_[i]; // копируем цифру в буфер
        }
        else if (ip_[i] == '.') { // если точка
            if (atoi(buff) > 255) // проверить диапазон октета
                return false;

            if (numbers == 0) // если числа нет - ошибка
                return false;

            numbers = 0; // сбрасываем счетчик чисел
            points++; // увеличиваем счетчик точек
            delete[] buff; // освобождаем память буфера
            buff = new char[3]; // выделяем память для нового октета
        }
        else {
            return false; // если символ не цифра и не точка – ошибка
        }
    }

    // Проверка на количество точек и число в последнем октете
    if (points != 3) return false; // если точек не 3 - ошибка
    if (numbers == 0 || numbers > 3) return false; // ошибка в последнем октете

    return true; // IP-адрес корректный
}

// Функция для преобразования IP-адреса в 32-битное число
unsigned long CharToLong(char* ip_) {
    unsigned long out = 0; // число для IP-адреса
    char* buff = new char[3]; // буфер для хранения одного октета

    // Проход по каждому символу IP-адреса
    for (int i = 0, j = 0, k = 0; ip_[i] != '\0'; i++, j++) {
        if (ip_[i] != '.') { // если не точка
            buff[j] = ip_[i]; // записать символ в буфер
        }

        // Если встретили точку или достигли конца строки
        if (ip_[i] == '.' || ip_[i + 1] == '\0') {
            out <<= 8; // сдвинуть число на 8 бит влево

            if (atoi(buff) > 255) // если октет больше 255 – ошибка
                return NULL;

            out += (unsigned long)atoi(buff); // преобразовать и добавить к IP-числу
            k++; // увеличиваем счетчик октетов
            j = -1; // сбрасываем индекс буфера
            delete[] buff; // освобождаем память буфера
            buff = new char[3]; // выделяем новую память для буфера
        }
    }
    return out; // возвращаем 32-битное число
}

// Функция для проверки корректности маски подсети
bool CheckMask(unsigned long mask) {
    if (mask == 0) // маска не может быть нулевой
        return false;

    unsigned long maskTest = 1 << 31; // начальная маска (1 0000 0000 ...)

    // Проверка, чтобы не было 0 после единиц в маске
    for (int i = 0; i < 32; i++) {
        if (!(mask & maskTest)) { // если текущий бит равен 0
            for (int j = i + 1; j < 32; j++) {
                mask <<= 1; // сдвигаем маску влево
                if (mask & maskTest) // если встретили 1
                    return false; // ошибка, маска некорректна
            }
        }
        mask <<= 1; // продолжаем сдвигать маску
    }
    return true; // маска корректна
}

// Функция для вывода 32-битного числа в формате IP-адреса
void WriteLongToChar(unsigned long ip) {
    unsigned long oktet[4]; // массив для хранения октетов
    oktet[0] = ip >> 24; // первый октет
    oktet[1] = (ip << 8) >> 24; // второй октет
    oktet[2] = (ip << 16) >> 24; // третий октет
    oktet[3] = (ip << 24) >> 24; // четвертый октет

    // Вывод IP-адреса
    cout << oktet[0] << "." << oktet[1] << "." << oktet[2] << "." << oktet[3] << "\n";
}

int main() {
    setlocale(LC_ALL, "Russian"); // Установка русской локали

    unsigned long ip, mask, host, subnet, broadcast; // Переменные для хранения данных
    char* ip_, * mask_; // Указатели для строк IP-адреса и маски
    bool flag = true; // Флаг для проверки правильности ввода
    ip_ = new char[16]; // Выделение памяти для ввода IP-адреса
    mask_ = new char[16]; // Выделение памяти для ввода маски подсети

    // Цикл ввода IP-адреса
    do {
        if (!flag) cout << "\nНеверно введён адрес!\n" << endl; // Сообщение об ошибке
        cout << "Введите IP - адрес: \n> "; // Запрос ввода
        cin >> ip_; // Ввод IP-адреса
    } while (!(flag = CheckAddress(ip_))); // Проверка корректности адреса

    // Преобразование IP-адреса в 32-битное число
    ip = CharToLong(ip_);
    flag = true; // Сброс флага для ввода маски

    // Цикл ввода маски подсети
    do {
        if (!flag)
            cout << "\nНеправильная маска!\n" << endl; // Сообщение об ошибке
        flag = true; // Сброс флага
        do {
            if (!flag)
                cout << "\nНеверно введена маска!\n" << endl; // Сообщение об ошибке
            cout << "Введите маску подсети: \n> "; // Запрос ввода
            cin >> mask_; // Ввод маски
        } while (!(flag = CheckAddress(mask_))); // Проверка корректности маски
        mask = CharToLong(mask_); // Преобразование маски в 32-битное число
    } while (!(flag = CheckMask(mask))); // Проверка корректности маски подсети

    // Вычисление ID подсети, ID хоста и Broadcast адреса
    subnet = ip & mask; // ID подсети
    host = ip & ~mask; // ID хоста
    broadcast = ip & mask | ~mask; // Broadcast адрес

    // Вывод результатов
    cout << "ID подсети:\t\t";
    WriteLongToChar(subnet); // Вывод ID подсети

    cout << "ID хоста:\t\t";
    WriteLongToChar(host); // Вывод ID хоста

    cout << "Broadcast адрес:\t";
    WriteLongToChar(broadcast); // Вывод Broadcast адреса

    // Освобождение выделенной памяти
    delete[] ip_;
    delete[] mask_;

    return 0; // Завершение программы
}
