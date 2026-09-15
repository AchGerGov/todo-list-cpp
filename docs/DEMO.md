# Сценарий сдачи проекта

## 1. Трекер задач
https://github.com/users/AchGerGov/projects — 5 колонок, 15 задач с assignee, сроком, приоритетом и % выполнения.

## 2. Репозиторий
https://github.com/AchGerGov/todo-list-cpp — ветки main и develop, README, docs/AUDIT.md.

## 3. CI
https://github.com/AchGerGov/todo-list-cpp/actions — зелёные прогоны на main и develop.

## 4. Сборка
git clone git@github.com:AchGerGov/todo-list-cpp.git
cd todo-list-cpp
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON
cmake --build build -j
cd build && ctest --output-on-failure

Ожидаемо: 100% tests passed, 0 tests failed out of 9.

## 5. Демонстрация фич
./build/todo-app
  add Купить молоко | 2 литра
  tag 1 home
  due 1 2026-01-15
  prio 1 high
  list
  search молок
  filter prio high
  save /tmp/tasks.txt
  exit

./build/todo-app /tmp/tasks.txt
  list
  exit
