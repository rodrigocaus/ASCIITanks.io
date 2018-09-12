Rodrigo Caus (186807) e Victor Santolim (187888)
Lab 4 - portaudio

Mudanças do lab 4:

	Foram adicionados em relação ao lab 3 os arquivos 01-playback para gerenciamento
	de áudio. As alterações feitas em 01-playback são apenas no tamanho do buffer. No
	model_mainloop, foram adicionados os objetos Audio::Sample e Audio::Player para
	abrir o arquivo de áudio. Toda vez que detectada uma tecla 'w' ou 's', é emitido
	um som (dando um player, quando essas teclas são tocadas pela primeira vez ou
	voltando o cursor de áudio para 0 com asample->set_position(0)). Por fim, fechamos
	o canal de áudio com o player->stop().
