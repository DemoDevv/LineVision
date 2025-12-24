#pragma once

/**
 * @enum EAN8Error
 * @brief Codes d'erreur retournés par les fonctions de la bibliothèque EAN-8.
 */
typedef enum {
    /** @brief Aucune erreur. Opération réussie. */
    EAN8_ERROR_NONE = 0,
    /** @brief Échec d'allocation mémoire (malloc/calloc). */
    EAN8_ERROR_MEMORY_ALLOCATION = 1,
    /** @brief Argument invalide passé à la fonction (ex: pointeur NULL). */
    EAN8_ERROR_INVALID_INPUT = 2,
    /** @brief Le chiffre de contrôle (8ème digit) est incorrect. */
    EAN8_ERROR_INVALID_CHECKSUM = 3,
    /** @brief Format du segment EAN-8 invalide (mauvaise taille ou marqueurs absents). */
    EAN8_ERROR_INVALID_FORMAT = 4,
    /** @brief Échec du décodage des barres en chiffres (motif inconnu). */
    EAN8_ERROR_INVALID_DECODE = 5
} EAN8Error;

/**
 * @brief Convertit un code d'erreur EAN8Error en une chaîne de caractères lisible.
 * @param error Le code d'erreur à traduire.
 * @return Un pointeur vers une chaîne de caractères constante décrivant l'erreur.
 */
const char* ean8_error_to_string(EAN8Error error);
