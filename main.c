#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_MESSAGES 100

/* 定义用户结构体 */
typedef struct {
    char username[50];
    char password[50];
} User;

/* 定义留言结构体 */
typedef struct {
    int id;
    char author[50];
    char content[200];
} Message;

/* 全局变量：存用户和留言 */
User users[MAX_USERS];
int user_count = 0;

Message messages[MAX_MESSAGES];
int message_count = 0;
int next_message_id = 1; /* 留言的ID，自增 */

int is_logged_in = 0;
char current_username[50];

/* 保存数据到文件 */
void save_data() {
    FILE *fp;
    int i;

    /* 保存用户 */
    fp = fopen("users.txt", "w");
    if (fp != NULL) {
        for (i = 0; i < user_count; i++) {
            fprintf(fp, "%s %s\n", users[i].username, users[i].password);
        }
        fclose(fp);
    }

    /* 保存留言 */
    fp = fopen("messages.txt", "w");
    if (fp != NULL) {
        for (i = 0; i < message_count; i++) {
            fprintf(fp, "%d|%s|%s\n", messages[i].id, messages[i].author, messages[i].content);
        }
        fclose(fp);
    }
}

/* 从文件加载数据 */
void load_data() {
    FILE *fp;
    int i;

    /* 加载用户 */
    fp = fopen("users.txt", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%s %s", users[user_count].username, users[user_count].password) == 2) {
            user_count++;
            if (user_count >= MAX_USERS) break;
        }
        fclose(fp);
    }

    /* 加载留言 */
    fp = fopen("messages.txt", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%d|%49[^|]|%199[^\n]", &messages[message_count].id, messages[message_count].author, messages[message_count].content) == 3) {
            if (messages[message_count].id >= next_message_id) {
                next_message_id = messages[message_count].id + 1;
            }
            message_count++;
            if (message_count >= MAX_MESSAGES) break;
        }
        fclose(fp);
    }
}

int main() {
    int choice;
    
    /* 程序启动时，先把之前保存的数据读进来 */
    load_data();

    while (1) {
        printf("\n===== 留言板 =====\n");
        printf("1. 注册\n");
        printf("2. 登录\n");
        printf("3. 发布留言\n");
        printf("4. 查看留言\n");
        printf("5. 退出\n");
        printf("请选择: ");

        if (scanf("%d", &choice) != 1) {
            printf("输入错误，请输入数字！\n");
            while (getchar() != '\n'); /* 清空输入缓冲区 */
            continue;
        }

        switch (choice) {
            case 1: {
                int exists = 0;
                int i;
                char name[50], pwd[50];
                
                printf("请输入用户名: ");
                scanf("%s", name);
                printf("请输入密码: ");
                scanf("%s", pwd);

                for (i = 0; i < user_count; i++) {
                    if (strcmp(users[i].username, name) == 0) {
                        exists = 1;
                        break;
                    }
                }

                if (exists) {
                    printf("注册失败：用户名已存在！\n");
                } else {
                    strcpy(users[user_count].username, name);
                    strcpy(users[user_count].password, pwd);
                    user_count++;
                    save_data(); /* 保存到文件 */
                    printf("注册成功！请返回菜单登录。\n");
                }
                break;
            }
            case 2: {
                int found = 0;
                int i;
                char name[50], pwd[50];
                
                printf("请输入用户名: ");
                scanf("%s", name);
                printf("请输入密码: ");
                scanf("%s", pwd);

                for (i = 0; i < user_count; i++) {
                    if (strcmp(users[i].username, name) == 0 && strcmp(users[i].password, pwd) == 0) {
                        found = 1;
                        break;
                    }
                }

                if (found) {
                    is_logged_in = 1;
                    strcpy(current_username, name);
                    printf("登录成功！欢迎 %s\n", name);
                } else {
                    printf("登录失败：用户名或密码错误！\n");
                }
                break;
            }
            case 3: {
                /* 把声明放到最上面，防止VC++ 2010报错 */
                char content[200]; 
                
                if (!is_logged_in) {
                    printf("请先登录再发布留言！\n");
                    break;
                }
                
                printf("请输入留言内容（不要有空格）: ");
                scanf("%s", content);
                
                messages[message_count].id = next_message_id++;
                strcpy(messages[message_count].author, current_username);
                strcpy(messages[message_count].content, content);
                message_count++;
                
                save_data(); /* 保存到文件 */
                printf("留言发布成功！\n");
                break;
            }
            case 4: {
                int i;
                if (message_count == 0) {
                    printf("暂时没有任何留言。\n");
                    break;
                }
                printf("\n----- 所有留言 -----\n");
                for (i = 0; i < message_count; i++) {
                    printf("[%d] %s: %s\n", messages[i].id, messages[i].author, messages[i].content);
                }
                printf("--------------------\n");
                break;
            }
            case 5:
                //printf("再见！\n");
                return 0;
            default:
                printf("没有这个选项，请重新选择。\n");
        }
    }

    return 0;
}