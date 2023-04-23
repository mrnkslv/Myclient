#pragma comment (lib,"ws2_32.lib")
#include<winsock2.h>
#include<iostream>
#include <fstream>
#include<string>

#pragma warning (disable:4996)

void recv_file(SOCKET* sock) {
	char file_size_str[16];
	char file_name[32];

	recv(*sock, file_size_str, 16, 0);
	int file_size = atoi(file_size_str);
	char* bytes = new char[file_size];

	recv(*sock, file_name, 32, 0);

	std::fstream file;
	file.open(file_name, std::ios_base::out | std::ios_base::binary);

	std::cout << "size: " << file_size << std::endl;
	std::cout << "name: " << file_name << std::endl;

	if (file.is_open()) {

		recv(*sock, bytes, file_size, 0);
		std::cout << "data: " << bytes << std::endl;

		file.write(bytes, file_size);
		std::cout << "Saved" << std::endl;
	}
	else {
		std::cout << "Error in opening file \n";
	}
	delete[]bytes;
	file.close();
}
int main() {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "error" << std::endl;
		exit(1);
	}
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "failed connection to server \n";
		return 1;
	}
	else {
		std::cout << " connected. \n";
		recv_file(&Connection);
	}
	system("pause");
	return 0;
}