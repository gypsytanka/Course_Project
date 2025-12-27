#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "blog.h"

int main() {
    // Выделение динамической памяти для массива блогов
    blogs* blog;
    blog = (blogs*)malloc(MAX_BLOGS * sizeof(blogs));
    int records_count = 0;  // Счетчик текущих записей
    system("chcp 1251");

    int choice;  // Переменная для выбора пункта меню
    int result;
    struct Blog new_blog;
    int category_choice;
    int status_choice;
    int search_type;
    char search_author[50];
    double min_rating;
    char filename[100];
    int num_records;

    printf("=== Разработка программы работы с файловой базой данных “Блог” ===\n");
    printf("Выполнила: Маслова Татьяна бТИИ-251\n");

    do {
        printf("\n=== Система управления блогом ===\n");
        printf("Максимальное количество записей: %d\n", MAX_BLOGS);
        printf("\nТекущее количество записей: %d/%d\n", records_count, MAX_BLOGS);
        printf("1. Просмотреть все записи\n");
        printf("2. Добавить новую запись\n");
        printf("3. Поиск статей\n");
        printf("4. Отсортировать данные\n");
        printf("5. Сохранить данные в файл\n");
        printf("6. Загрузить данные из файла\n");
        printf("7. Создать тестовые данные\n");
        printf("0. Выход\n");
        printf("Выберите пункт меню: ");

        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            printf("\n--- Просмотр всех записей ---\n");
            result = view_all(blog, records_count);
            if (result == 0) {
                printf("Нет записей для отображения.\n");
                printf("Сначала добавьте записи через пункт меню 'Добавить новую запись'.\n");
            }
            else {
                printf("Всего записей: %d (максимум %d)\n", result, MAX_BLOGS);
            }
            break;
        case 2:
            printf("\n--- Добавление новой записи ---\n");

            if (records_count >= MAX_BLOGS) {
                printf("Достигнуто максимальное количество записей (%d).\n", MAX_BLOGS);
                break;
            }

            printf("Введите заголовок статьи: ");
            fgets(new_blog.title, sizeof(new_blog.title), stdin);
            new_blog.title[strcspn(new_blog.title, "\n")] = 0;

            printf("Введите автора: ");
            fgets(new_blog.author, sizeof(new_blog.author), stdin);
            new_blog.author[strcspn(new_blog.author, "\n")] = 0;

            printf("Выберите категорию:\n");
            printf("0 - Технологии\n");
            printf("1 - Наука\n");
            printf("2 - Искусство\n");
            printf("3 - Образование\n");
            printf("4 - Другое\n");
            printf("Введите номер категории: ");
            scanf("%d", &category_choice);
            getchar();

            switch (category_choice) {
            case 0: new_blog.category = TECHNOLOGY; break;
            case 1: new_blog.category = SCIENCE; break;
            case 2: new_blog.category = ART; break;
            case 3: new_blog.category = EDUCATION; break;
            case 4: new_blog.category = OTHER; break;
            default: new_blog.category = OTHER;
            }

            printf("Введите дату публикации:\n");
            printf("День: ");
            scanf("%d", &new_blog.date.day);
            printf("Месяц: ");
            scanf("%d", &new_blog.date.month);
            printf("Год: ");
            scanf("%d", &new_blog.date.year);
            getchar();

            printf("Введите количество комментариев: ");
            scanf("%d", &new_blog.comments);
            getchar();

            printf("Введите рейтинг: ");
            scanf("%lf", &new_blog.rating);
            getchar();

            printf("Выберите статус модерации:\n");
            printf("0 - На модерации\n");
            printf("1 - Одобрено\n");
            printf("2 - Отклонено\n");
            printf("Введите номер статуса: ");
            scanf("%d", &status_choice);
            getchar();

            switch (status_choice) {
            case 0: new_blog.status = PENDING; break;
            case 1: new_blog.status = APPROVED; break;
            case 2: new_blog.status = REJECTED; break;
            default: new_blog.status = PENDING;
            }

            result = add_blog(blog, &records_count, &new_blog);

            if (result == 1) {
                printf("Запись успешно добавлена! Текущее количество записей: %d\n", records_count);
            }
            else if (result == -1) {
                printf("Ошибка: достигнут лимит записей!\n");
            }
            break;
        case 3:
            printf("\n=== ПОИСК СТАТЕЙ ===\n");
            printf("Выберите тип поиска:\n");
            printf("1 - Поиск по категории\n");
            printf("2 - Комбинированный поиск (автор + минимальный рейтинг)\n");
            printf("Ваш выбор: ");
            scanf("%d", &search_type);
            while (getchar() != '\n');

            if (search_type == 1) {
                printf("\n--- Поиск по категории ---\n");
                printf("Выберите категорию:\n");
                printf("0 - Технологии\n");
                printf("1 - Наука\n");
                printf("2 - Искусство\n");
                printf("3 - Образование\n");
                printf("4 - Другое\n");
                printf("Введите номер категории: ");
                scanf("%d", &category_choice);
                while (getchar() != '\n');

                result = search_by_range(blog, records_count, search_type, category_choice, "", 0.0);
                if (result == -1) {
                    printf("Ошибка: неверный номер категории.\n");
                }
                else if (result == -2) {
                    printf("Ошибка: неверный тип поиска.\n");
                }
                else if (result == 0) {
                    printf("По вашему запросу ничего не найдено.\n");
                }
                else {
                    printf("Найдено записей: %d\n", result);
                }
            }
            else if (search_type == 2) {
                printf("\n--- Комбинированный поиск ---\n");
                printf("Введите автора для поиска: ");
                fgets(search_author, sizeof(search_author), stdin);
                search_author[strcspn(search_author, "\n")] = 0;

                printf("Введите минимальный рейтинг: ");
                scanf("%lf", &min_rating);
                while (getchar() != '\n');

                result = search_by_range(blog, records_count, search_type, 0, search_author, min_rating);
                if (result == -1) {
                    printf("Ошибка: неверный номер категории.\n");
                }
                else if (result == -2) {
                    printf("Ошибка: неверный тип поиска.\n");
                }
                else if (result == 0) {
                    printf("По вашему запросу ничего не найдено.\n");
                }
                else {
                    printf("Найдено записей: %d\n", result);
                }
            }
            else {
                printf("Ошибка: неверный тип поиска.\n");
            }
            break;
        case 4:
            printf("\n--- Сортировка данных ---\n");
            result = sort_data(blog, records_count);
            if (result == 0) {
                printf("Нет записей для сортировки.\n");
            }
            else {
                printf("Количество перестановок: %d\n", result);
            }
            break;
        case 5:
            printf("\n=== СОХРАНЕНИЕ ДАННЫХ В ФАЙЛ ===\n");
            printf("Введите имя файла для сохранения: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;

            result = save_to_file(blog, records_count, filename);
            if (result == 0) {
                printf("Нет записей для сохранения.\n");
            }
            else if (result == -1) {
                printf("Ошибка открытия файла для записи.\n");
            }
            else {
                printf("Данные успешно сохранены. Сохранено записей: %d\n", result);
            }
            break;
        case 6:
            printf("\n=== ЗАГРУЗКА ДАННЫХ ИЗ ФАЙЛА ===\n");

            if (records_count >= MAX_BLOGS) {
                printf("Достигнут лимит записей (%d).\n", MAX_BLOGS);
                break;
            }

            printf("Введите имя файла: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            // Сохраняем количество записей до загрузки
            int before_load = records_count;

            result = load_from_file(blog, &records_count, filename, &new_blog);

            if (result == -1) {
                printf("Ошибка открытия файла '%s'.\n", filename);
            }
            else if (result == 0) {
                printf("Файл не содержит корректных записей или пуст.\n");
            }
            else {
                int added_count = records_count - before_load;
                printf("Загружено записей: %d\n", added_count);
                printf("Всего записей стало: %d (до загрузки было: %d)\n", records_count, before_load);
                printf("Доступно места: %d записей\n", MAX_BLOGS - records_count);
            }
            break;
        case 7:
            printf("\n=== Создание тестовых данных ===\n");
            printf("Сколько тестовых записей создать? (1-%d): ", MAX_BLOGS - records_count);
            scanf("%d", &num_records);
            while (getchar() != '\n');

            result = create_test(blog, &records_count, &new_blog, num_records);
            if (result > 0) {
                printf("Создано %d тестовых записей. Всего записей: %d\n", result, records_count);
            }
            else if (result == -1) {
                printf("Ошибка: количество записей должно быть не менее 1\n");
            }
            else if (result == -2) {
                printf("Ошибка: не хватает места. Доступно только %d записей\n", MAX_BLOGS - records_count);
            }
            break;
        case 0:
            printf("\nВыход из программы. До свидания!\n");
            break;
        default:
            printf("\nОшибка: неверный пункт меню.\n");
            break;
        }

        if (choice != 0) {
            printf("\nНажмите Enter для продолжения...");
            getchar();
        }

    } while (choice != 0);

    free(blog);
    return 0;
}