#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include "blog.h"

// Функция для получения текстового представления категории
char* get_category(enum Category category) {
    switch (category) {
    case TECHNOLOGY: return "Технологии";
    case SCIENCE: return "Наука";
    case ART: return "Искусство";
    case EDUCATION: return "Образование";
    case OTHER: return "Другое";
    default: return "Неизвестно";
    }
}

// Функция для получения текстового представления статуса модерации
char* get_status(enum ModerStatus status) {
    switch (status) {
    case PENDING: return "На модерации";
    case APPROVED: return "Одобрено";
    case REJECTED: return "Отклонено";
    default: return "Неизвестно";
    }
}

// Функция для отображения одной записи блога
int display_blog(struct Blog blog, int record_number) {
    printf("\n--- Запись %d ---\n", record_number);
    printf("Заголовок: %s\n", blog.title);
    printf("Автор: %s\n", blog.author);
    printf("Категория: %s\n", get_category(blog.category));
    printf("Дата публикации: %02d.%02d.%d\n",
        blog.date.day,
        blog.date.month,
        blog.date.year);
    printf("Количество комментариев: %d\n", blog.comments);
    printf("Рейтинг: %.1f\n", blog.rating);
    printf("Статус модерации: %s\n", get_status(blog.status));
    printf("------------------------\n");
    return 1;
}

int view_all(blogs* blog, int records_count) {

    if (records_count == 0) {
        return 0;
    }
    for (int i = 0; i < records_count; i++) {
        display_blog(blog[i], i + 1);
    }
    return records_count;
}

int add_blog(blogs* blog, int* records_count, struct Blog* new_blog) {
    if (*records_count >= MAX_BLOGS) {
        return -1;
    }

    blog[*records_count] = *new_blog;
    (*records_count)++;
    return 1;
}

int search_by_range(blogs* blog, int records_count, int search_type,
    int category_choice, char* search_author, double min_rating) {
    int found_count = 0; // Счетчик найденных записей

    if (search_type == 1) {
        if (category_choice < 0 || category_choice > 4) {
            return -1;
        }

        for (int i = 0; i < records_count; i++) {
            if (blog[i].category == category_choice) {
                display_blog(blog[i], i + 1);
                found_count++;
            }
        }
    }
    else if (search_type == 2) {
        for (int i = 0; i < records_count; i++) {
            if (strcmp(blog[i].author, search_author) == 0 &&
                blog[i].rating >= min_rating) {
                display_blog(blog[i], i + 1);
                found_count++;
            }
        }
    }
    else {
        return -2;
    }

    return found_count;
}

int sort_data(blogs* blog, int records_count) {
    if (records_count == 0) {
        return 0;
    }

    int swap_count = 0; // Счетчик кол-ва перестановок
    // Сортировка пузырьком
    for (int i = 0; i < records_count - 1; i++) {
        for (int j = 0; j < records_count - i - 1; j++) {
            int need_swap = 0;
            if (blog[j].rating < blog[j + 1].rating) {
                need_swap = 1;
            }
            // Если рейтинги равны, переходим к следующему критерию
            else if (blog[j].rating == blog[j + 1].rating) {
                if (blog[j].date.year < blog[j + 1].date.year) {
                    need_swap = 1;
                }
                else if (blog[j].date.year == blog[j + 1].date.year) {// Если годы равны, сравниваем месяцы
                    if (blog[j].date.month < blog[j + 1].date.month) {
                        need_swap = 1;
                    }
                    else if (blog[j].date.month == blog[j + 1].date.month) {// Если месяцы равны, сравниваем дни
                        if (blog[j].date.day < blog[j + 1].date.day) {
                            need_swap = 1;
                        }
                        // Если даты равны, сравниваем категории
                        else if (blog[j].date.day == blog[j + 1].date.day) {// Сравнение категорий по алфавитному порядку их названий
                            // Получаем текстовые представления категорий
                            const char* cat1 = get_category(blog[j].category);
                            const char* cat2 = get_category(blog[j + 1].category);
                            // Сравниваем строки категорий с помощью strcmp
                            // strcmp возвращает:
                            // < 0 если cat1 < cat2 (по алфавиту)
                            // = 0 если cat1 == cat2
                            // > 0 если cat1 > cat2 (по алфавиту)
                            if (strcmp(cat1, cat2) > 0) {
                                need_swap = 1;  // Категория в обратном алфавитном порядке - меняем
                            }
                        }
                    }
                }
            }

            if (need_swap) {
                struct Blog temp = blog[j];
                blog[j] = blog[j + 1]; // Обмен значениями между текущим и следующим элементом
                blog[j + 1] = temp;
                swap_count++;
            }
        }
    }
    for (int i = 0; i < records_count; i++) {
        display_blog(blog[i], i + 1);
    }
    return swap_count;
}

int save_to_file(blogs* blog, int records_count, char* filename) {
    int saved_count = 0;

    if (records_count == 0) {
        return 0;
    }

    FILE* file = fopen(filename, "wt");
    if (file == NULL) {
        return -1;
    }
    for (int i = 0; i < records_count; i++) {
        fprintf(file, "Заголовок:%s\n", blog[i].title);
        fprintf(file, "Автор:%s\n", blog[i].author);
        fprintf(file, "Категория:%d\n", blog[i].category);
        fprintf(file, "Дата:%d:%d:%d\n", blog[i].date.day, blog[i].date.month, blog[i].date.year);
        fprintf(file, "Комментарии:%d\n", blog[i].comments);
        fprintf(file, "Рейтинг:%.1f\n", blog[i].rating);
        fprintf(file, "Статус:%d\n", blog[i].status);
        fprintf(file, "---\n");  // Разделитель между записями

        saved_count++;;
    }

    fclose(file);
    return saved_count;
}

int load_from_file(blogs* blog, int* records_count, char* filename, struct Blog* new_blog) {
    char line[256];
    int field_count = 0; //счетчик полей
    int loaded_count = 0;

    FILE* file = fopen(filename, "rt");
    if (file == NULL) {
        return -1;
    }

    int current_count = *records_count; // Сохраняем текущее количество записей

    while (fgets(line, sizeof(line), file) != NULL && *records_count < MAX_BLOGS) {
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) continue;
        // Пропускаем разделитель между записями
        if (strcmp(line, "---") == 0) {
            if (field_count == 7) {
                blog[*records_count] = *new_blog;
                (*records_count)++;
                loaded_count++;
                field_count = 0;
            }
            continue;
        }
        // Чтение полей
        if (strncmp(line, "Заголовок:", 10) == 0) {
            strcpy(new_blog->title, line + 10);
            field_count++;
        }
        else if (strncmp(line, "Автор:", 6) == 0) {
            strcpy(new_blog->author, line + 6);
            field_count++;
        }
        else if (strncmp(line, "Категория:", 10) == 0) {
            new_blog->category = atoi(line + 10);
            field_count++;
        }
        else if (strncmp(line, "Дата:", 5) == 0) {
            sscanf(line + 5, "%d:%d:%d",
                &new_blog->date.day,
                &new_blog->date.month,
                &new_blog->date.year);
            field_count++;
        }
        else if (strncmp(line, "Комментарии:", 12) == 0) {
            new_blog->comments = atoi(line + 12);
            field_count++;
        }
        else if (strncmp(line, "Рейтинг:", 8) == 0) {
            new_blog->rating = atof(line + 8);
            field_count++;
        }
        else if (strncmp(line, "Статус:", 7) == 0) {
            new_blog->status = atoi(line + 7);
            field_count++;
        }
    }
    // Обработка последней записи, если файл не заканчивается разделителем
    if (field_count == 7) {
        blog[*records_count] = *new_blog;
        (*records_count)++;
        loaded_count++;
    }

    fclose(file);
    return loaded_count;
}

int create_test(blogs* blog, int* records_count, struct Blog* new_blog, int num_records) {
    if (num_records < 1) {
        return -1;
    }

    if (*records_count + num_records > MAX_BLOGS) {
        return -2;
    }
    srand(time(NULL));  // Инициализация генератора случайных чисел

    // Массивы тестовых данных
    char* authors[] = {
        "Иван Иванов", "Петр Петров", "Анна Сидорова","Мария Кузнецова", "Алексей Смирнов", "Ольга Васильева",
        "Дмитрий Попов", "Елена Новикова", "Сергей Морозов","Наталья Волкова"
    };

    char* titles[] = {
         "Искусственный интеллект будущее","Квантовые компьютеры сегодня","Живопись эпохи Возрождения","Современные методы обучения",
         "Исследования космоса 2024","Цифровое искусство и NFT","Программирование для начинающих","Научные открытия года",
         "Технологии в медицине","История мирового искусства"
    };

    for (int i = 0; i < num_records; i++) {

        strcpy(new_blog->title, titles[rand() % 10]);
        strcpy(new_blog->author, authors[rand() % 10]);
        new_blog->category = rand() % 5;
        new_blog->date.day = 1 + rand() % 28;
        new_blog->date.month = 1 + rand() % 12;
        new_blog->date.year = 2020 + rand() % 5;
        new_blog->comments = rand() % 1000;
        new_blog->rating = 1.0 + (rand() % 91) / 10.0;
        new_blog->status = rand() % 3;

        blog[*records_count] = *new_blog;
        (*records_count)++;
    }
    return num_records;
}