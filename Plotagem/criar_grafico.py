from cProfile import label
from curses import color_content
from math import log2
import matplotlib.pyplot as plt
import numpy as np

pontos_lista_x = []
pontos_lista_y = []
pontos_arvore_x = []
pontos_arvore_y = []
pontos_maior_comparacao_lista_x = []
pontos_maior_comparacao_lista_y = []
pontos_maior_comparacao_arvore_x = []
pontos_maior_comparacao_arvore_y = []

x_lista_math = np.linspace(0, 60000, 100)
img_lista_math = x_lista_math

x_arvore_math = np.linspace(1, 60000, 100)
img_arvore_math = []
for i in x_arvore_math:
    img_arvore_math.append(log2(i))

with open('dados.txt', 'r') as file:
    while True:
        line = file.readline()

        if not line:
            break

        line = line.split(' ')

        pontos_lista_x.append(int(line[0]))
        pontos_lista_y.append(int(line[1]))

        pontos_arvore_x.append(int(line[0]))
        pontos_arvore_y.append(int(line[2]))

with open('maiores_comparacoes.txt', 'r') as file:
    while True:
        line = file.readline()

        if not line:
            break

        line = line.split(' ')

        pontos_maior_comparacao_lista_x.append(int(line[0]))
        pontos_maior_comparacao_lista_y.append(int(line[1]))

        pontos_maior_comparacao_arvore_x.append(int(line[0]))
        pontos_maior_comparacao_arvore_y.append(int(line[2]))


plt.title('Lista Encadeada vs Árvore Binária')
plt.xlabel('Número de nós')
plt.ylabel('Comparações')

plt.grid(True, 'major', 'both')

plt.plot(x_arvore_math, img_arvore_math, label='O(log2(n))')
plt.plot(pontos_maior_comparacao_lista_x, pontos_maior_comparacao_lista_y, label='Maior comparação possível para Lista')
plt.plot(pontos_maior_comparacao_arvore_x, pontos_maior_comparacao_arvore_y, label='Maior comparação possível para Árvore')
plt.plot(pontos_lista_x, pontos_lista_y, '.', label='Lista Encadeada')
plt.plot(pontos_arvore_x, pontos_arvore_y, '.', label='Árvore Binária')

plt.legend()

plt.show()