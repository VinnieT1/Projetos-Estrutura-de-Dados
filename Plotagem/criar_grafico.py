from curses import color_content
import matplotlib.pyplot as plt

pontos_lista_x = []
pontos_lista_y = []
pontos_arvore_x = []
pontos_arvore_y = []

with open('dados.txt', 'r') as file:
    while True:
        line = file.readline().removesuffix('\n')

        if not line:
            break

        line = line.split(' ')

        pontos_lista_x.append(line[0])
        pontos_lista_y.append(line[1])

        pontos_arvore_x.append(line[0])
        pontos_arvore_y.append(line[2])


plt.title('Lista Encadeada vs Árvore Binária')
plt.xlabel('Número de nós')
plt.ylabel('Comparações')


plt.xticks()
plt.yticks()

plt.grid(True, color='k', linestyle=':')
plt.plot(pontos_lista_x, pontos_lista_y, 'r.')
plt.plot(pontos_arvore_x, pontos_arvore_y, 'b.')


plt.show()