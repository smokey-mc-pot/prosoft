  <pre>
  Решение тестового задания для компании Prosoft

  prosoft                       # Корневая папка проекта
  ├── cmake/                    # Настройки и флаги для сборки
  │   ├── common_flags.cmake    # Общие флаги компиляции для clang
  │   └── unique_flags.cmake    # Уникальные флаги для windows и linux и компиляторов gcc, msvc, clang
  │
  ├── documentation/            # Документы с описаниями решений
  │   ├── task_1_solution.docx
  │   ├── task_2_solution.docx
  │   ├── task_3_solution.docx
  │   ├── task_5_solution.docx
  │   └── task_6.txt
  │
  ├── task_1/                   # Исходники задачи 1
  ├── task_2/                   # Исходники задачи 2
  ├── task_3/                   # Исходники задачи 3
  ├── task_4/                   # Исходники задачи 4
  ├── task_5/                   # Исходники задачи 5
  │   ├── extra_find_avx512.cpp # Решение задачи 5 с использованием AVX512, есть небольшой прирост производительности            
  │        
  │
  ├── task_6/                   # Исходники задачи 6
  │
  ├── .clang-format             # Форматирование кода
  ├── .gitattributes            # Атрибуты Git
  ├── .gitignore                # Игнорируемые файлы/папки
  ├── CMakeLists.txt            # Главный скрипт сборки
  ├── CMakePresets.json         # Пресеты конфигураций для VS
  └── README.md                 # Описание проекта
  </pre>
