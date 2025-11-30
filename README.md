Решается система уравнений

<img width="299" height="161" alt="image" src="https://github.com/user-attachments/assets/7a4fd146-ac75-465e-8f1e-b50df6fa5601" />

Для этого используется метод Рунге-Кутты

<img width="459" height="228" alt="image" src="https://github.com/user-attachments/assets/bd73cded-5feb-4cd8-9f32-1e5d7f516f15" />
<img width="453" height="231" alt="image" src="https://github.com/user-attachments/assets/23e2a360-62bc-48be-8381-fefc1ccd640b" />

Для распараллеливания циклов используется директива #pragma omp parallel for. Имеет смысл использовать только статическое разделение итераций цикла между потоками, так как на каждой иттерации выполняется одинаковое количество операций.

Для верификации программы были построены траектории движения тел при разных условиях.
Два тела: Масса первого тела - 5.972e24 кг, масса второго – 2.986e24 кг. Оба тела имеют скорость 511,5 м/с, направленны скорости в противоположные стороны относительно друга.

<img width="210" height="155" alt="image" src="https://github.com/user-attachments/assets/95c330d5-6ddd-491e-b2db-f046575fa174" />

Три тела:

<img width="185" height="140" alt="image" src="https://github.com/user-attachments/assets/5ce04581-e2c5-4729-8046-ed5b191a3347" />
<img width="181" height="140" alt="image" src="https://github.com/user-attachments/assets/8e406b10-e604-41cd-beb1-a3058a1ef737" />
<img width="185" height="140" alt="image" src="https://github.com/user-attachments/assets/445f5245-da3a-44a5-981f-0f8f6a751864" />

10 тел:

<img width="204" height="157" alt="image" src="https://github.com/user-attachments/assets/bb8c723a-67e4-4afe-ba6e-1dcd390340bd" />

Для расчета эффективности программы решалась задача для 200 тел. Расчетное время подобрано таким образом, чтобы на одном потоке программа считала примерно 30 секунд. Количество физических ядер на процессоре – 16.

<img width="1152" height="223" alt="image" src="https://github.com/user-attachments/assets/753c4234-1129-40b1-b9b7-502b2a0d4603" />
<img width="1084" height="876" alt="image" src="https://github.com/user-attachments/assets/0a5b8b64-156b-4a92-acfd-ec3e0cfd19b3" />
