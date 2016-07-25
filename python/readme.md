# Surfing Services em Python

## Reprodução básica das operações básicas do Surfing Services. 

O Script escreve na portal serial o comando "sensors" o qual o servidor serial na surfboard pega todos os valores atualizados do sensores e responde em um json formatado com a chave (nome sensor) e o valor atualizado. Com o retorno do JSON via serial da IoT Surfboard o mesmo é salvo em um banco NoSQL (MongoDB) para futuras avaliações analíticas. O mesmo json é publicado no Broker MQTT configurado.

Necessário instalar algumas dependências caso ainda não estejam ja instaladas em seu ambiente.
para isso utilize: 

Exemplo:
pip install pymongo

# Telegram Bot em Python

## Comunicação da Iot Surfbord com um bot no Telegram.

O script fica ouvindo a API do Telegram e dependendo dos comandos passados pelo usuário que esta com o bot aberto no Aplicativo Telegram o bot comunica com a placa buscando valores atualizado dos sensores ou enviando comando para os atores.

Para mais informações de como utilizar a API do Telegram, procure a documentação deles.

Para ambos scripts mudem para serem executaveis:
> chmod +x Sensors.py

depois execute em daemon

> ./Sensores.py &
