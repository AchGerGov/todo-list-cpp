# Журнал ревизии исходного кода

Проект: https://github.com/FujiwaraChoki/todo-list-cpp  
Дата ревизии: 2026-09-15  
Исполнитель: AchGerGov

| # | Проблема | Критичность | Статус | Решение |
|---|----------|-------------|--------|---------|
| 1 | Отсутствует система сборки | High | Fixed | Добавлен CMake |
| 2 | Нет тестов | High | Fixed | Google Test, 9 тестов |
| 3 | Утечки памяти (new без delete) | High | Fixed | RAII, std::vector |
| 4 | Смешение логики и UI | Medium | Fixed | Разделение на Task / TodoList / ConsoleUI |
| 5 | Нет валидации ввода | Medium | Fixed | Проверки в ConsoleUI::processCommand |
| 6 | Нет документации | Low | Fixed | README + help в CLI |
| 7 | Нет CI | Medium | Fixed | GitHub Actions |
| 8 | Не используется C++17 | Low | Fixed | enum class, std::optional, noexcept |
| 9 | Нет .gitignore | Low | Fixed | Добавлен |
| 10 | Регистрозависимый парсер приоритетов | High | Fixed | Сделан регистронезависимым |

## Реализованные функции

1. Приоритеты (Low / Medium / High)
2. Дедлайны (формат YYYY-MM-DD)
3. Теги (категории)
4. Поиск и фильтры
5. Сохранение / загрузка в файл
