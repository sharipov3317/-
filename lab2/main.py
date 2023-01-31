# Функция для вычисления количества контрольных битов
def calculate(length):
    power = len(bin(length)[2:]) # наибольшая степень двойки в двоичном представлении числа
    if length + power < 2 ** power:
        number_of_control_bits = power
    else:
        number_of_control_bits = power + 1
    return number_of_control_bits

# функция для кодирования битовой последовательности
def hamming(sequence, length):
    number_of_control_bits = calculate(length) 
    encrypted_length = length + number_of_control_bits

    a = []
    s0 = []
    for i in range(number_of_control_bits):
        a.append([])
        s0.append(0)
    
    array_for_encrypted_sequence = [0] * encrypted_length
    i = 0

    for j in range(1, encrypted_length):
        # Проверьте индекс на наличие двойника в n-й степени
        if j & (j + 1) != 0: 
            array_for_encrypted_sequence[j] = int(sequence[i])
            i += 1

    # Создайте матрицу контрольных сумм и
    # Вектор контрольных сумм, соответствующий исходной последовательности
    for i in range(number_of_control_bits):
        a[i] = [0] * encrypted_length
        k = 0

        for j in range(2 ** i - 1, encrypted_length):
            binary_representation_of_j = bin(j + 1)[2:] 
            power_position = len(binary_representation_of_j) - i - 1
            if binary_representation_of_j[power_position] == '1':
                a[i][j] = 1
                if array_for_encrypted_sequence[j] == 1: 
                    k += 1
        s0[i] = k % 2
        
    i = 0
    transmitted_sequence = '' 
    for j in range(encrypted_length):
        # Проверьте индекс на наличие двойника в n-й степени
        if j & (j + 1) == 0:
            array_for_encrypted_sequence[j] = s0[i]
            i += 1
        transmitted_sequence += ''.join(str(array_for_encrypted_sequence[j]))

    for i in range(len(a)):
        print(f"a{i}: {a[i]}")

    print(f"Начальный вектор контрольных сумм s0: {s0}")

    st = []
    for i in a:
        st_k = 0      
        # давайте умножим элементы i строки массива a на элементы массива закодированной последовательности
        for j in range(len(i)):
            st_k += i[j] * array_for_encrypted_sequence[j] 
        st.append(st_k % 2)

    print(f"Переданный вектор контрольной суммы st: {st}")

    # Если st не равно [0] * number_of_control_bits, завершите программу
    if st != [0] * number_of_control_bits:
        print("Ошибка, что-то пошло не так.")
        exit(1)
    return transmitted_sequence

# Функция для проверки переданной последовательности битов
def check(sequence):
    length = len(sequence)
    number_of_control_bits = len(bin(length)[2:]) 
    s0 = [0] * number_of_control_bits
    sr = [0] * number_of_control_bits
    array_for_encrypted_sequence = [0] * length

    i = 0
    for j in range(length):
        if j & (j + 1) == 0:
            sr[i] = int(sequence[j])
            i += 1
        else:
            array_for_encrypted_sequence[j] = int(sequence[j])

    # Создайте матрицу контрольных сумм и
    # Вектор контрольной суммы, соответствующий передаваемой битовой последовательности
    for i in range(number_of_control_bits):
        k = 0

        for j in range(2 ** i - 1, length):
            binary_representation_of_j = bin(j + 1)[2:]
            power_position = len(binary_representation_of_j) - i - 1
            if binary_representation_of_j[power_position] == '1':
                if array_for_encrypted_sequence[j] == 1: 
                    k += 1
        s0[i] = k % 2
    
    position = '' # Номер позиции ошибки в двоичном представлении
    for i in range(number_of_control_bits):
        if sr[i] != s0[i]:
            position += '1'
        else:
            position += '0'

    print(f"Вектор контрольной суммы ошибочной последовательности sr: {sr}")
    print(f"Исходная контрольная сумма равна s0: {s0}")

    return int(position[::-1], 2)

def main():
    sequence = input(f"Введите исходную последовательность: ")
    length = len(sequence) # Длина исходной битовой последовательности
    
    print("Закодированная последовательность:", hamming(sequence, length))

    transmitted_sequence = input(f"Передаваемая последовательность: ")

    # Функция для поиска байта с ошибкой
    position = check(transmitted_sequence)
    if position == 0:
        print("В переданной битовой последовательности ошибок не обнаружено")
    else:
        print(f"В переданной битовой последовательности обнаружена ошибка. Его позиция такова {position}")

if __name__ == "__main__":
    main()