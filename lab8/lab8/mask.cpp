#include <iostream>
using namespace std;

// ������� ��� �������� ������������ ���������� IP-������
bool CheckAddress(char* ip_) {
    int points = 0, // ���������� �����
        numbers = 0; // �������� ������

    char* buff; // ����� ��� ������ ������
    buff = new char[3]; // �������� ������ ��� ������ (����. 3 �����)

    // ������ �� ������� ������� IP-������
    for (int i = 0; ip_[i] != '\0'; i++) {
        if (ip_[i] <= '9' && ip_[i] >= '0') { // ���� �����
            if (numbers > 3) return false; // ���� ������ ���� ����� � ������ � ������
            buff[numbers++] = ip_[i]; // �������� ����� � �����
        }
        else if (ip_[i] == '.') { // ���� �����
            if (atoi(buff) > 255) // ��������� �������� ������
                return false;

            if (numbers == 0) // ���� ����� ��� - ������
                return false;

            numbers = 0; // ���������� ������� �����
            points++; // ����������� ������� �����
            delete[] buff; // ����������� ������ ������
            buff = new char[3]; // �������� ������ ��� ������ ������
        }
        else {
            return false; // ���� ������ �� ����� � �� ����� � ������
        }
    }

    // �������� �� ���������� ����� � ����� � ��������� ������
    if (points != 3) return false; // ���� ����� �� 3 - ������
    if (numbers == 0 || numbers > 3) return false; // ������ � ��������� ������

    return true; // IP-����� ����������
}

// ������� ��� �������������� IP-������ � 32-������ �����
unsigned long CharToLong(char* ip_) {
    unsigned long out = 0; // ����� ��� IP-������
    char* buff = new char[3]; // ����� ��� �������� ������ ������

    // ������ �� ������� ������� IP-������
    for (int i = 0, j = 0, k = 0; ip_[i] != '\0'; i++, j++) {
        if (ip_[i] != '.') { // ���� �� �����
            buff[j] = ip_[i]; // �������� ������ � �����
        }

        // ���� ��������� ����� ��� �������� ����� ������
        if (ip_[i] == '.' || ip_[i + 1] == '\0') {
            out <<= 8; // �������� ����� �� 8 ��� �����

            if (atoi(buff) > 255) // ���� ����� ������ 255 � ������
                return NULL;

            out += (unsigned long)atoi(buff); // ������������� � �������� � IP-�����
            k++; // ����������� ������� �������
            j = -1; // ���������� ������ ������
            delete[] buff; // ����������� ������ ������
            buff = new char[3]; // �������� ����� ������ ��� ������
        }
    }
    return out; // ���������� 32-������ �����
}

// ������� ��� �������� ������������ ����� �������
bool CheckMask(unsigned long mask) {
    if (mask == 0) // ����� �� ����� ���� �������
        return false;

    unsigned long maskTest = 1 << 31; // ��������� ����� (1 0000 0000 ...)

    // ��������, ����� �� ���� 0 ����� ������ � �����
    for (int i = 0; i < 32; i++) {
        if (!(mask & maskTest)) { // ���� ������� ��� ����� 0
            for (int j = i + 1; j < 32; j++) {
                mask <<= 1; // �������� ����� �����
                if (mask & maskTest) // ���� ��������� 1
                    return false; // ������, ����� �����������
            }
        }
        mask <<= 1; // ���������� �������� �����
    }
    return true; // ����� ���������
}

// ������� ��� ������ 32-������� ����� � ������� IP-������
void WriteLongToChar(unsigned long ip) {
    unsigned long oktet[4]; // ������ ��� �������� �������
    oktet[0] = ip >> 24; // ������ �����
    oktet[1] = (ip << 8) >> 24; // ������ �����
    oktet[2] = (ip << 16) >> 24; // ������ �����
    oktet[3] = (ip << 24) >> 24; // ��������� �����

    // ����� IP-������
    cout << oktet[0] << "." << oktet[1] << "." << oktet[2] << "." << oktet[3] << "\n";
}

int main() {
    setlocale(LC_ALL, "Russian"); // ��������� ������� ������

    unsigned long ip, mask, host, subnet, broadcast; // ���������� ��� �������� ������
    char* ip_, * mask_; // ��������� ��� ����� IP-������ � �����
    bool flag = true; // ���� ��� �������� ������������ �����
    ip_ = new char[16]; // ��������� ������ ��� ����� IP-������
    mask_ = new char[16]; // ��������� ������ ��� ����� ����� �������

    // ���� ����� IP-������
    do {
        if (!flag) cout << "\n������� ����� �����!\n" << endl; // ��������� �� ������
        cout << "������� IP - �����: \n> "; // ������ �����
        cin >> ip_; // ���� IP-������
    } while (!(flag = CheckAddress(ip_))); // �������� ������������ ������

    // �������������� IP-������ � 32-������ �����
    ip = CharToLong(ip_);
    flag = true; // ����� ����� ��� ����� �����

    // ���� ����� ����� �������
    do {
        if (!flag)
            cout << "\n������������ �����!\n" << endl; // ��������� �� ������
        flag = true; // ����� �����
        do {
            if (!flag)
                cout << "\n������� ������� �����!\n" << endl; // ��������� �� ������
            cout << "������� ����� �������: \n> "; // ������ �����
            cin >> mask_; // ���� �����
        } while (!(flag = CheckAddress(mask_))); // �������� ������������ �����
        mask = CharToLong(mask_); // �������������� ����� � 32-������ �����
    } while (!(flag = CheckMask(mask))); // �������� ������������ ����� �������

    // ���������� ID �������, ID ����� � Broadcast ������
    subnet = ip & mask; // ID �������
    host = ip & ~mask; // ID �����
    broadcast = ip & mask | ~mask; // Broadcast �����

    // ����� �����������
    cout << "ID �������:\t\t";
    WriteLongToChar(subnet); // ����� ID �������

    cout << "ID �����:\t\t";
    WriteLongToChar(host); // ����� ID �����

    cout << "Broadcast �����:\t";
    WriteLongToChar(broadcast); // ����� Broadcast ������

    // ������������ ���������� ������
    delete[] ip_;
    delete[] mask_;

    return 0; // ���������� ���������
}
