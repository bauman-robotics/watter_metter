Стм-ка просыпается по будильнику каждую минуту.
Будет есп-шку на 30 сек. Есп-шка пытается установить связь с сервером, 
если ок, запрашивает по уарту показания счетчиков воды у стм-ки.
Если ок, есп-шка передает по уарту команду - установить будильник 
на другое время, можно раз в сутки (сейчас на две минуты). 
Есп - передает команду - сбросить текущее время в ноль.
Плюс  esp -передает команду - выключить себе питание.
Если пакет не прошел до сервера - стм-ка каждую минуту будет есп-шку 
на 30 сек и ждёт успешной отправки пакета. Если отправка пакета прошла 
успешно, итнтервал будильника - меняется, и стм-ка будит есп-шку - через 
другой интервал времени, который прилетел ей от esp-шки.
Установил время антидребезга на 3000 ms.
Срабатывание только по заднему фронту входных сигналов (переход: 1 -> 0)