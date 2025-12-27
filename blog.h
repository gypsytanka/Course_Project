#ifndef BLOG_H
#define BLOG_H
#define MAX_BLOGS 50  // Максимальное количество записей в блоге

// Перечисление для категорий блогов
enum Category {
    TECHNOLOGY,
    SCIENCE,
    ART,
    EDUCATION,
    OTHER
};

// Перечисление для статуса модерации
enum ModerStatus {
    PENDING,
    APPROVED,
    REJECTED
};

// Структура для хранения даты
struct Date {
    int day;
    int month;
    int year;
};

// Структура для хранения информации о записи
typedef struct Blog {
    char title[100];
    char author[50];
    enum Category category;
    struct Date date;
    int comments;
    double rating;
    enum ModerStatus status;
} blogs;

/**
 * Получение текстового представления категории статьи
 * @param category - числовой код категории (тип enum Category)
 * @return указатель на строку-константу (текстовое название категории)
 */
char* get_category(enum Category category);

/**
 * Получение текстового представления статуса модерации
 * @param status - числовой код категории (тип enum ModerStatus)
 * @return указатель на строку-константу (текстовое название статуса)
 */
char* get_status(enum ModerStatus status);

/**
 * Отображение одной записи
 * @param blog - значение структуры Blog
 * @param record_number - номер записи для отображеня
 * @return 1, указывая на успешное выполнение
 */
int display_blog(struct Blog blog, int record_number);

/**
 * Просмотр всех записей
 * @param blog - указатель на массив структур Blog
 * @param records_count - счетчик записей
 * @return количество отображенных записей и 0 если записей нет
 */
int view_all(blogs* blog, int records_count);

/**
 * Добавление новой записи
 * @param blog - указатель на массив структур Blog
 * @param records_count - указатель на счетчик записей
 * @param new_blog - указатель на структуру с данными новой записи
 * @return 1 при успешном добавлении, -1 если достигнут лимит записей
 */
int add_blog(blogs* blog, int* records_count, struct Blog* new_blog);

/**
 * Поиск записей по категории или по автору+минимальному рейтингу
 * @param blog - указатель на массив структур Blog
 * @param records_count -  счетчик записей
 * @param search_type - тип поиска (1 - по категории, 2 - по автору и рейтингу)
 * @param category_choice - номер категории для поиска (используется при search_type=1)
 * @param search_author - имя автора для поиска (используется при search_type=2)
 * @param min_rating - минимальный рейтинг для поиска (используется при search_type=2)
 * @return количество найденных записей, 0 если не найдено, -1/-2 при ошибках ввода
 */
int search_by_range(blogs* blog, int records_count, int search_type,
    int category_choice, char* search_author, double min_rating);

/**
 * Сортировка данных по рейтингу, дате и категории
 * @param blog - указатель на массив структур Blog
 * @param records_count - указатель на счетчик записей
 * @return количество выполненных перестановок элементов (0 если сортировать нечего)
 * Сортирует по рейтингу (убывание), дате (убывание) и категории (алфавитно)
 */
int sort_data(blogs* blog, int records_count);

/**
 * Сохранение данных в файл
 * @param blog - указатель на массив структур Blog
 * @param records_count - счетчик записей
 * @param filename - имя файла для сохранения
 * @return количество сохраненных записей, 0 если нечего сохранять, -1 при ошибке файла
 */
int save_to_file(blogs* blog, int records_count, char* filename);

/**
 * Загрузка данных из файла
 * @param blog - указатель на массив структур Blog
 * @param records_count - указатель на счетчик записей
 * @param filename - имя файла для загрузки
 * @param new_blog - указатель на структуру для чтения данных
 * @return количество загруженных записей, -1 при ошибке открытия файла
 * Примечание: записи добавляются к существующим, не заменяют их
 */
int load_from_file(blogs* blog, int* records_count, char* filename, struct Blog* new_blog);

/**
 * Создание тестовых данных
 * @param blog - указатель на массив структур Blog
 * @param records_count - указатель на счетчик записей
 * @param new_blog - указатель на структуру для заполнения данными
 * @param num_records - кол-во создаваемых тестовых данных
 * @return количество созданных записей (-1/-2 при ошибках ввода)
 */
int create_test(blogs* blog, int* records_count, struct Blog* new_blog, int num_records);

#endif 
