### Вывод чисел в диапазоне от -99 до 255 с шагом 1 на 3 семисегментных индикатора

<table>
  <tr>
    <th>I0</th>
    <th>I1</th>
    <th>I2</th>
    <th>Диапазон</th>
    <th>I0</th>
    <th>I1</th>
    <th>I2</th>
  </tr>
  <tr>
    <td align="center">-</td>
    <td align="center">9</td>
    <td align="center">9</td>
    <td align="center" rowspan="3">[-99; -10]</td>
    <td align="center" rowspan="3">s</td>
    <td align="center" rowspan="3">d1</td>
    <td align="center" rowspan="3">d0</td>
  </tr>
  <tr>
    <td align="center" colspan="3">...</td>
  </tr>
  <tr>
    <td align="center">-</td>
    <td align="center">1</td>
    <td align="center">0</td>
  </tr>
  <tr>
    <td align="center"></td>
    <td align="center">-</td>
    <td align="center">9</td>
    <td align="center" rowspan="3">(-10; 0)</td>
    <td align="center" rowspan="3"></td>
    <td align="center" rowspan="3">s</td>
    <td align="center" rowspan="3">d0</td>
  </tr>
  <tr>
    <td align="center" colspan="3">...</td>
  </tr>
  <tr>
    <td align="center"></td>
    <td align="center">-</td>
    <td align="center">1</td>
  </tr>
  <tr>
    <td align="center"></td>
    <td align="center"></td>
    <td align="center">0</td>
    <td align="center" rowspan="3">[0; 10)</td>
    <td align="center" rowspan="3"></td>
    <td align="center" rowspan="3">s</td>
    <td align="center" rowspan="3">d0</td>
  </tr>
  <tr>
    <td align="center" colspan="3">...</td>
  </tr>
  <tr>
    <td align="center"></td>
    <td align="center"></td>
    <td align="center">9</td>
  </tr>
  <tr>
    <td align="center"></td>
    <td align="center">1</td>
    <td align="center">0</td>
    <td align="center" rowspan="3">[10; 100)</td>
    <td align="center" rowspan="3">s</td>
    <td align="center" rowspan="3">d1</td>
    <td align="center" rowspan="3">d0</td>
  </tr>
  <tr>
    <td align="center" colspan="3">...</td>
  </tr>
  <tr>
    <td align="center"></td>
    <td align="center">9</td>
    <td align="center">9</td>
  </tr>
  <tr>
    <td align="center">1</td>
    <td align="center">0</td>
    <td align="center">0</td>
    <td align="center" rowspan="3">[100; 255]</td>
    <td align="center" rowspan="3">d2</td>
    <td align="center" rowspan="3">d1</td>
    <td align="center" rowspan="3">d0</td>
  </tr>
  <tr>
    <td align="center" colspan="3">...</td>
  </tr>
  <tr>
    <td align="center">2</td>
    <td align="center">5</td>
    <td align="center">5</td>
  </tr>
</table>

#### Обозначения

**I0, I1, I2** - индикаторы (слева направо)

**d2** - разряд сотен

**d1** - разряд десятков

**d0** - разряд единиц

**s** - символ "минус", если число отрицательное; пустой символ, если число положительное или ноль
