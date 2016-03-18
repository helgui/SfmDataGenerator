# SfmDataGenerator
## Обзор
**SfmDataGenerator** позволяет создавать реалистичные наборы данных для задачи разреженной реконструкции (*Struture from Motion, SfM*). Набор данных формируется
в процессе виртуальной съемки 3D-модели, представленной в формате [PLY](http://graphics.stanford.edu/data/3Dscanrep/#file_format).
Использование **SfmDataGenerator** позволяет
 * Избежать ресурсоемкого поиска *особых точек* (*features*) и *соответствий* между ними (*feature matching*)
 * Работать со сценами произвольной геометрии и произвольного характера съемки
 * Производить оценку качества алгоритмов реконструкции благодаря наличию эталонного результата (*ground truth*)
 * Исследовать чувствительность алгоритмов реконструкции к зашумленности входных данных
## Зависимости
## Сборка
## Запуск
PLY модели можно загрузить с сайтов:
 * [The Stanford 3D Scanning Repository](http://graphics.stanford.edu/data/3Dscanrep)
 * [Large geometric models archive](http://www.cc.gatech.edu/projects/large_models)
 * [KSCAN3D Gallery](http://www.kscan3d.com/gallery)
 * [PLY Files an ASCII Polygon Format](http://people.sc.fsu.edu/~jburkardt/data/ply/ply.html)
##Лицензия
##Автор
Олег Яковлев
*для связи с автором используйте*
<iframe id="forum_embed"
  src="javascript:void(0)"
  scrolling="no"
  frameborder="0"
  width="900"
  height="700">
</iframe>
<script type="text/javascript">
  document.getElementById('forum_embed').src =
     'https://groups.google.com/forum/embed/?place=forum/sfmdatagen'
     + '&showsearch=true&showpopout=true&showtabs=false'
     + '&parenturl=' + encodeURIComponent(window.location.href);
</script>
