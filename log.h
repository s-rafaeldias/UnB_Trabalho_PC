//
// Created by rafael on 30/05/16.
//

#ifndef TRABALHOPC_LOG_H
#define TRABALHOPC_LOG_H

#ifndef STRUCT_LOG
#define STRUCT_LOG

typedef struct {
    int hora;
    int prod;
}Log;

#endif // STRUCT_LOG

#ifndef STRUCT_LOG_MAQ
#define STRUCT_LOG_MAQ

typedef struct {
    int id;
    Log log;
}Historico;

#endif // STRUCT_LOG_MAQ

#endif // TRABALHOPC_LOG_H
