# ALGORITMO K-MEANS PARA COMPUTACIÓN DE ALTAS PRESTACIONES

## Idea general

El objetivo de este proyecto es implementar el algoritmo K-means aplicandole diversas optimizaciones para reducir su tiempo de ejecución (speedup). Este algoritmo es muy utilizado en minería de datos, y consiste en ordenar conjuntos de puntos entorno a varios centroides, de forma que cada punto quede agrupado a su centroide más cercano. Para ello se desarrollará el programa en C++ y se aplicarán técnicas de optimización del código y paralelización mediante software (MPI) y hardware (openMP).

## Mejoras implementadas

- Uso de la memoria contigua mediante un vector con las coordenadas de los puntos para aprovechar el uso de la caché.
- Paralelización a través de hardware (openMP).
- Paralelización a través de software (MPI).


## Estructura del Proyecto

```text
📦 HPC-KMeans
 ┣ 📂 data/         # Carpeta para datos binarios
 ┣ 📂 src/          # Código fuente principal
 ┣ 📂 include/      # Archivos de cabecera (.h)
 ┗ 📜 README.md

```

## Ejecución del proyecto