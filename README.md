# jogadores

# como fazer o codigo funcionar

## instalando os requisitos para rodar o mysql no linux do codespace

sudo apt-get update
sudo apt-get install mysql-server libmysqlclient-dev -y

## para iniciar o banco

sudo service mysql start

## para criar o banco de dados, usuario e tabela

sudo mysql

### cria o banco de dados

CREATE DATABASE db_futebol;

### cria o usuario
CREATE USER 'admin_futebol'@'localhost' IDENTIFIED BY 'senha123';
GRANT ALL PRIVILEGES ON db_futebol.* TO 'admin_futebol'@'localhost';
FLUSH PRIVILEGES;

### cria a tabela
USE db_futebol;

CREATE TABLE jogadores (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nome VARCHAR(100) NOT NULL,
    posicao VARCHAR(50) NOT NULL,
    data_nascimento DATE NOT NULL,
    gols_marcados INT DEFAULT 0
);

## para executar o codigo
### para sair do mysql
EXIT;

### para compilar o codigo

gcc jogadores.c -o jogadores_app -lmysqlclient

### para executar o codigo

./jogadores_app