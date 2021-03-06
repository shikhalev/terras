## Terras

Проект на Arduino для контроля температуры/влажности и т.д. в нескольких террариумах. Используются дешевые датчики по шине I²C через мультиплексор.

### Основная функциональность

#### Первый этап

- [x] Получение данных с датчиков и вывод на экран.
  - [ ] Разобраться с экранными шрифтами, чтоб выводить знак градуса.
- [ ] Удобная передача данных через Serial на компьютер.
  - [ ] Клиентская программа для получения (анализа?) этих данных.

#### Второй этап

Буду реализовывать по мере приобретения компонент.

- [ ] Сохранение настроек (сейчас в константах) и управление ими с энкодера.
- [ ] Замена экрана на более крупный.
- [ ] Энергосбережение (включая гашение экрана).

#### Третий этап

Буду реализовывать по мене приобретения компонент.

- [ ] Автоматическое управление микроклиматом:
  - [ ] Принудительная вентиляция,
  - [ ] Обогрев,
  - [ ] Охлаждение,
  - [ ] Увлажнение (и уменьшение влажности через вентиляцию).

### Дополнительно

Параллельно основным этапам в произвольном порядке.

- [ ] Оптимизация обращения к датчикам в плане расходования оперативной памяти.
- [ ] Разводка печатных плат.
- [ ] Корпуса.
