#include <iostream>
#include <string>

int main(int argc, char** argv) {
  int choicedNumber = -1;
  std::string authorizationTime, roleType, companyName;
  std::cout << "请选择注册权限（1.试用，2.年费，3.终身会员）：" << std::endl;
  std::cout << "填入对应的数字：";
  std::cin >> choicedNumber;
  switch (choicedNumber) {
    case 1:
      std::cout << "试用版" << std::endl;
      break;
    case 2:
      std::cout << "年费版" << std::endl;
      break;
    case 3:
      std::cout << "终身免费版" << std::endl;
      break;
    default:
      std::cout << "选项错误，请选择正确的选项。" << std::endl;
      break;
  }
  return 0;
}