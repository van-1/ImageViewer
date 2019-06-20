Компонент отображает в полноэкранном режиме изображения различных форматов (bmp, png, jpg, все остальные, которые поддерживаются библиотекой qt5). С изображением можно выполнить ряд действий: увеличить масштаб (ctrl+, crtl +  вращение колеса мышки), уменьшить масштаб (ctrl-, ctrl + вращение колеса мышки в обратную строну), перемещение по увеличенному изображению с помощью стрелок и мышки (с зажатой левой клавишей), быстрый переход к начальному размеру изображения (crtl + 0).

В дистрибутив компонента включена демо-программа, в которой приведен пример использования компонента. Приложение требует наличие Qt 5 для сборки. 

```bash
cmake <...> -IMGVIEWER_BUILD_DEMO=ON
```

## Зависимости

Компонент разработан на языке C++ на основе библиотеки Qt5. Для сборки необходимы компилятор, совместимый со стандартом C++11, CMake 3.1+ и библиотека Qt версии 5.9+.
