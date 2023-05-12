#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

typedef struct CarNode{
    struct CarNode* left;
    struct CarNode* right;
    int data;
    int time;
    bool isMember;
    double grade;
    int where;
}CarNode;

int counter;

CarNode* Insert(CarNode* node, int data, bool isMember, double grade, int where){
    time_t time = 0;
    if(node == NULL){
        CarNode* newNode = (CarNode*)malloc(sizeof(CarNode));
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->data = data;
        newNode->time = time;
        newNode->isMember = isMember;
        newNode->grade = grade;
        newNode->where = where;
        
        return newNode;
    } else {
        if(node->data > data) {
            node->left = Insert(node->left, data, isMember, grade, where);
        } else if(node->data < data) {
            node->right = Insert(node->right, data, isMember, grade, where);
        } else {
        	printf("이미 주차된 차량입니다. 주차된 차량 번호 : %d", data);
        }
        return node;
    }
}

void Modify(CarNode* node, int data, int inputdata){
    if(node == NULL){
        return;
    }

    if(node->data == data){
        node->data = inputdata;
    } else {
        if(node->data > data) {
            Modify(node->left, data, inputdata);
        }
        else {
            Modify(node->right, data, inputdata);
        }
    }
}

CarNode* find_node(CarNode* node, int data){
    if(node == NULL){
        return NULL;
    }
    
    if(node->data == data){
        return node;
    } else {
        if(node->data > data) {
            return find_node(node->left, data);
        } else {
            return find_node(node->right, data);
        }
    }
}

CarNode* get_min(CarNode* node, CarNode** minNode) {
    if(node->left == NULL){
        *minNode = node;
        node = node->right;
        return node;
    } else {
        node->left = get_min(node->left, minNode);
        return node;
    }
}

CarNode* get_max(CarNode* node, CarNode** maxNode) {
    if(node->right == NULL){
        *maxNode = node;
        node = node->left;
        return node;
    } else {
        node->right = get_max(node->right, maxNode);
        return node;
    }
}

CarNode* delete_node(CarNode* node, int data, char* parkingArray) {
    if(node == NULL){
        return NULL;
    }
    
    if(node->data == data){
        CarNode* deleteNode = node;
        if(node->left == NULL && node->right == NULL) {
            parkingArray[node->where-1] = 0;
            node = NULL;
        } else if(node->left != NULL && node->right == NULL) {
            node = node->left;
        } else if(node->left == NULL && node->right != NULL) {
            node = node->right;
        } else {
            CarNode* minNode = NULL;
            node->right = get_min(node->right, &minNode);
            minNode->left = deleteNode->left;
            minNode->right = deleteNode->right;
            node = minNode;
        }
        
        free(deleteNode);
        return node;
    } else {
        if(node->data > data) {
            node->left = delete_node(node->left, data, parkingArray);
        } else {
            node->right = delete_node(node->right, data, parkingArray);
        }
        return node;
    }
}

void inorder_traversal(CarNode* node){
    if (node == NULL){
        return;
    }
    inorder_traversal(node->left);
    printf("%d ", node->data);
    inorder_traversal(node->right);
}

void right_root_left_traversal(CarNode* node){
    if(node == NULL){
        return;
    }
    right_root_left_traversal(node->right);
    printf("%d ", node->data);
    right_root_left_traversal(node->left);
}

void delete(char* str, char c) {
    int len = strlen(str);
    int i, j;
    for (i = 0, j = 0; i < len; i++) {
        if (str[i] != c) {
            str[j] = str[i];
            j++;
        }
    }
    str[j] = '\0';
}

int checker(char* parkingArray){
    int index = -1;
    int size = sizeof(parkingArray) / sizeof(int); 
    for (int i = 0; i < size; i++) {
        if (parkingArray[i] == 0) {
            index = i; // 0이 나타난 인덱스 저장
            break; // 첫 번째로 나타난 0을 찾았으므로 반복문 종료
        }
    }
    return index+1;
}

wchar_t extract_korean_char(const wchar_t *str) {
    int len = wcslen(str); // 문자열 길이 계산
    wchar_t ch = L'\0'; // 반환할 한글 글자
    for (int i = 0; i < len; i++) {
        if (str[i] >= 0xAC00 && str[i] <= 0xD7A3) { // 한글 유니코드 범위 판별
            ch = str[i]; // 한글 글자 저장
            break; // 첫 번째로 발견한 한글 글자 반환
        }
    }
    return ch; // 반환한 한글 글자 반환
}

int extract_number(const wchar_t *str) {
    int len = wcslen(str); // 문자열 길이 계산
    int result = 0; // 결과값 저장 변수
    for (int i = 0; i < len; i++) {
        if (iswdigit(str[i])) { // 숫자인 경우
            result = result * 10 + (str[i] - L'0'); // 결과값 업데이트
        }
    }
    return result;
}

long long int combine_int(int a, wchar_t b) {
    char str[40]; // 최대 10자리 정수 2개를 합친 문자열의 길이는 최대 20
    sprintf(str, "%d%d", a, b); // 두 정수를 문자열로 변환하여 이어붙임
    long long result = atoll(str);
    return result; // 문자열을 정수로 변환하여 반환
}

int main(){
    setlocale(LC_ALL, ""); // 로케일 설정
	CarNode* mytree = NULL;
    int max_parking_car = 0;
    int carNumber = 0;
    int price_per_hour = 0;
    char* parkingArray;;
    int where = 1;
    bool isMember = false;
    float grade = 0.0;
    
    wchar_t str[100] = L"한글 번호판";
    char input[100];

    printf("성균관대학교 주차장 관리 프로그램 입니다. 소유하고 계신 주차장의 최대 주차가능 대수를 입력해주세요. ex) 2000\nconsole : ");
    scanf("%d", &max_parking_car);
    parkingArray = (char*)malloc(max_parking_car * sizeof(int));
    printf("시간당 가격을 입력하세요. ex)3000\nconsole : ");
    scanf("%d", &price_per_hour);
    printf("\n\n----------HOW TO USE----------");
    printf("\n해당 기능을 사용하기 위한 명령어는 다음과 같습니다.");
    printf("\n입차 : +");
    printf("\n출차 : -");
    printf("\n\n---------------------------------");

    printf("\n\n----------주차관리 프로그램----------");
    printf("\n총 수용가능 차량 : %d 대", max_parking_car);
    printf("\n시간당 주차비    : %d 원", price_per_hour);
    printf("\n---------------------------------");
    
    while(1){
        printf("\nconsole : ");
        scanf("%s", input);
        
        if(input[0] == '+'){
            //한글번호판 고유 숫자로 변경
            printf("차량 번호를 입력해주세요 : ");
            wscanf(L"%S", str);
            int onlyNum = extract_number(str);
            wchar_t hangle = extract_korean_char(str);
            long long int result = combine_int(onlyNum, hangle);

            //주차 번호 위치
            where = checker(parkingArray);
            parkingArray[where-1] = 1;

            //맴버쉽 bool 처리
            printf("멤버쉽이 적용되어 있나요? (Y/N) : ");
            scanf("%s", input);
            if(input[0] == 'Y' || input[0] == 'y'){
                isMember = true;
            }
            
            //학점 처리
            printf("학점을 적어주세요 ex) 4.2 : ");
            scanf("%f", &grade);

            mytree = Insert(mytree, result, isMember, grade, where);
            printf("------------주차 영수증------------");
            wprintf(L"\n차량번호  : %S", str);
            printf("\n주차코드    : %lld", result);
            printf(isMember ? "\n멤버적용대상 : 적용중" : "\n멤버적용대상 : 일반차량");
            printf("\n학점       : %lf", grade);
            printf("\n주차 위치   : %d", where);
            printf("\n-------------------------------");
        }

        if(input[0] == '-'){
            printf("출차할 차량 번호를 입력해주세요 : ");
            wscanf(L"%S", str);
            int onlyNum = extract_number(str);
            wchar_t hangle = extract_korean_char(str);
            long long int result = combine_int(onlyNum, hangle);
            mytree = delete_node(mytree, result, parkingArray);
        }
    }
}
