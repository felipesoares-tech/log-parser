# Log-Parser
Este algoritmo consiste em simular o funcionamento de um parser, onde iremos ler os logs gerados pelo jogo "Quake".

O arquivo "Quake.txt" é gerado pelo servidor de Quake 3 Arena. Nele está registrado informações sobre as partidas, informações como: Quando começa, quando termina, quem matou quem, quem "se matou" (caiu no vazio, explodiu a si próprio), entre outros. O Parser deve ser capaz de ler o arquivo, agrupar os dados de cada partida, e organizar as suas informações.

## Exemplo:

```txt
21:42 Kill: 1022 2 22: <world> killed Isgalamido by MOD_TRIGGER_HURT
* O player "Isgalamido" morreu por que estava ferido e caiu de uma altura que o matou.
 2:22 Kill: 3 2 10: Isgalamido killed Dono da Bola by MOD_RAILGUN
* O player "Isgalamido" matou o player "Dono da Bola" usando a arma "Railgun".
```
Para cada jogo o Parser deve gerar algo como:
```json
[{
  "game": 1,
  "status": {
     "total_kills": 43,
     "players": [
		{
			"id": 1,
			"nome": "Mocinha",
			"kills": 5,
			"old_names": ["Dono da bola"]
		},
		{
			"id": 2,
			"nome": "Isgalamido",
			"kills": 18,
			"old_names": []
		},
		{
			"id": 3,
			"nome": "Zeh",
			"kills": 20,
			"old_names": []
		}
	]
  }
}]
```


Observações: **Quando o &lt;world&gt; mata o player ele perde -1 kill. &lt;world&gt; não é um player e não deve aparecer na lista de players e nem no dicionário de kills. total_kills são os kills dos games, isso inclui as kills realizadas pelo &lt;world&gt;**


## Como executar
Para executar o código, será necessário utilizar o terminal, sendo passados 3 parâmetros, código fonte, local onde se encontra o log do game, e o local o qual irá salvá-lo.
Exemplo:
```sh
gcc parser.c -o parser -Wall
./parser input-files/Quake.txt output-files/saida.json
```
