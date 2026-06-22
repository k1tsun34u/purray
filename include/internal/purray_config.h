// internal/purray_config.h
/*
 * Internal configuration for Purray.
 * Do not include directly.
 */
#pragma once
#ifndef PURRAY_CONFIG_H
#define PURRAY_CONFIG_H

// ============================================================================
// Helper macros for converting to text
// (2 levels are needed to expand the macro's meaning, not its name)
// ============================================================================

#define PURRAY_STRINGIFY_INTERNAL(x)			#x
#define PURRAY_STRINGIFY(x)						PURRAY_STRINGIFY_INTERNAL(x)

#define PURRAY_VERSION_MAJOR					1
#define PURRAY_VERSION_MINOR					0
#define PURRAY_VERSION_PATCH					0

#define PURRAY_VERSION_STRING					\
	PURRAY_STRINGIFY(PURRAY_VERSION_MAJOR) "." \
	PURRAY_STRINGIFY(PURRAY_VERSION_MINOR) "." \
	PURRAY_STRINGIFY(PURRAY_VERSION_PATCH)

#define PURRAY_VERSION_NUMBER					\
	(PURRAY_VERSION_MAJOR * 10000 +				\
	PURRAY_VERSION_MINOR * 100 +				\
	PURRAY_VERSION_PATCH)

// ============================================================================
// Capacity rounding unit. Must be a power of two.
// Can be overridden by the user.
// ============================================================================

#ifndef PURRAY_CAPACITY_GRANULARITY
	#define PURRAY_CAPACITY_GRANULARITY				64
#elif (PURRAY_CAPACITY_GRANULARITY & (PURRAY_CAPACITY_GRANULARITY - 1)) != 0
	#error "PURRAY_CAPACITY_GRANULARITY must be a power of 2"
#endif // PURRAY_CAPACITY_GRANULARITY

// ============================================================================
// C11 features (_Generic support)
// ============================================================================

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
	#define PURRAY_USE_GENERIC
#endif // __STDC_VERSION__ >= 201112L

#endif // PURRAY_CONFIG_H
