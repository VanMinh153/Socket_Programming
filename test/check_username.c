#include <stdio.h>
#include <regex.h>

int is_valid_string(const char *str) {
    // Biểu thức regex
    const char *pattern = "^[a-zA-Z0-9]+(-[a-zA-Z0-9]+)*$";

    // Khởi tạo regex
    regex_t regex;
    int ret = regcomp(&regex, pattern, REG_EXTENDED);

    if (ret != 0) {
        fprintf(stderr, "Could not compile regex\n");
        return 0;
    }

    // Kiểm tra chuỗi
    ret = regexec(&regex, str, 0, NULL, 0);

    // Giải phóng regex
    regfree(&regex);

    // Trả về kết quả
    if (ret == 0) {
        return 1; // Chuỗi hợp lệ
    } else {
        return 0; // Chuỗi không hợp lệ
    }
}

int main() {
    // Test các chuỗi
    const char *test_strings[] = {
        "hello",
        "hello-world",
        "test123-abc",
        "-hello",
        "hello-",
        "hello--world",
        "hello@world",
        NULL // Kết thúc danh sách chuỗi
    };

    for (int i = 0; test_strings[i] != NULL; i++) {
        if (is_valid_string(test_strings[i])) {
            printf("'%s' is valid.\n", test_strings[i]);
        } else {
            printf("'%s' is invalid.\n", test_strings[i]);
        }
    }

    return 0;
}
