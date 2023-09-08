/* 
 * C
 * 
 * Copyright 2012-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
/* 
 * This file is auto-generated - DO NOT EDIT IT
 */
/**
 * GreenThread MicroJvm virtual machine API.
 */
#ifndef _LLMJVM
#define _LLMJVM 
#include <intern/LLMJVM.h>
#include <stdint.h>
#ifdef __cplusplus
	extern "C" {
#endif
// --------------------------------------------------------------------------------
// -                                  Constants                                   -
// --------------------------------------------------------------------------------

/**
 * Exit code returned when the application ends normally (i.e., all the non-daemon threads are terminated or <code>System.exit(exitCode)</code> has been called)
 */
#define LLMJVM_E_OK (0)

/**
 * Exit code returned when the SOAR used for the application is not compatible with the MicroJvm virtual machine.
 */
#define LLMJVM_E_SOAR_FILE_INCOMPATIBLE (-1)

/**
 * Exit code returned when the link specific configuration breaks MicroJvm virtual machine requirements.
 */
#define LLMJVM_E_BAD_LINK (-2)

/**
 * Exit code returned when the application ends because of the evaluation version limitations.
 */
#define LLMJVM_E_EVAL_LIMIT (-3)

/**
 * Exit code returned when the Main thread can not be launched (for instance if thread stack can not be allocated).
 */
#define LLMJVM_E_MAIN_THREAD_ALLOC (-5)

/**
 * Exit code returned when the specified maximum number of threads exceeds MicroJvm virtual machine limits.
 */
#define LLMJVM_E_TOO_MANY_THREADS (-12)

/**
 * Exit code returned when the Java heap size is too small or too large.
 */
#define LLMJVM_E_INV_HEAP_SIZE (-13)

/**
 * Exit code returned when the Java stacks memory is invalid. Check the stacks memory alignment or size.
 */
#define LLMJVM_E_INV_JAVA_STACK_MEM (-14)

/**
 * Exit code returned when the MicroJvm virtual machine cannot be restarted.
 */
#define LLMJVM_E_CANNOT_RESTART (-16)

/**
 * Exit code returned when the MicroJvm virtual machine is not in a valid state.
 */
#define LLMJVM_E_INV_STATE (-17)

/**
 * Exit code returned when the memory used for the Java heap or the immortal heap does not work properly. This may
 * be caused by an invalid external RAM configuration.
 */
#define LLMJVM_E_INV_HEAP_MEMORY (-18)

/**
 * Exit code returned when the memory used for the Java static fields does not work properly. This may
 * be caused by an invalid external RAM configuration.
 */
#define LLMJVM_E_INV_STATIC_MEMORY (-19)

/**
 * Exit code returned when KF rules configuration section is not correct. Check linker script files.
 */
#define LLMJVM_E_INV_KF_RULES_CONFIGURATION (-20)

/**
 * Exit code returned when the maximum number of monitors per threads exceeds MicroJvm virtual machine limits.
 */
#define LLMJVM_E_INV_TOO_MANY_MONITORS_PER_THREAD (-21)

/**
 * Exit code returned when FPU convention call requirements are not satisfied.
 */
#define LLMJVM_E_INV_FPU_CONVENTION_CALL (-22)

/**
 * Exit code returned when <code>LLMJVM_IMPL_initialize()</code> function defined in the abstraction layer returns an error.
 */
#define LLMJVM_E_INITIALIZE_ERROR (-23)

/**
 * Exit code returned when <code>LLMJVM_IMPL_vmTaskStarted()</code> function defined in the abstraction layer returns an error.
 */
#define LLMJVM_E_VM_TASK_STARTED_ERROR (-24)

/**
 * Exit code returned when <code>LLMJVM_IMPL_shutdown()</code> function defined in the abstraction layer returns an error.
 */
#define LLMJVM_E_SHUTDOWN_ERROR (-25)

/**
 * Error code returned when an error occurred.
 */
#define LLMJVM_E_ERROR (-1)

/**
 * Error code returned when an illegal argument has been given.
 */
#define LLMJVM_E_ILLEGAL_ARGUMENT (-2)

/**
 * Error code returned by {@link LLMJVM_suspendCurrentJavaThread} when suspend is not done.
 */
#define LLMJVM_E_INTERRUPTED (1)

// --------------------------------------------------------------------------------
// -                      Functions provided by the library                       -
// --------------------------------------------------------------------------------

/**
 * Initializes the MicroJvm virtual machine. This function MUST be called once before
 * a call to {@link LLMJVM_start}.
 */
void LLMJVM_initialize(void);

/**
 * Starts the MicroJvm virtual machine and call the <code>main</code> method of  the Java
 * application with the given String arguments. This function returns when the Java
 * application ends.
 * <p>
 * The Java application ends when there is no more Java thread to run or when the Java method
 * <code>System.exit(int)</code> is called.
 * 
 * @param argc java main argument count.
 * @param argv java main argument vector. An array of String (char**).
 * 
 * @return {@link LLMJVM_E_OK} when the MicroJvm virtual machine ends normally or a negative value
 * when an error occurred during startup.
 */
int32_t LLMJVM_start(int32_t argc, void* argv);

/**
 * Call this method after MicroJvm virtual machine execution to get the Java application exit code.
 * 
 * @return the value given to the <code>System.exit(exitCode)</code> or 0 if the Java application ends
 * without calling <code>System.exit(exitCode)</code>.
 */
int32_t LLMJVM_getExitCode(void);

/**
 * Prints the state of the MicroJvm virtual machine to the standard output stream.
 * For each Java thread, the Java stack trace, the name, the state and the priority are printed.
 * <p>
 * This function must only be called from the MicroJvm virtual machine thread context and only from a native function or callback.
 * Calling this function from another context may lead to undefined behavior and should be done only for debug purpose.
 */
void LLMJVM_dump(void);

/**
 * Checks internal structure integrity of MicroJvm virtual machine and returns its checksum.<br>
 * If an integrity error is detected, the <code>LLMJVM_on_CheckIntegrity_error</code> hook is called and this method returns <code>0</code>.<br>
 * If no integrity error is detected, a non-zero checksum is returned.
 * <p>
 * This function must only be called from the MicroJvm virtual machine thread context and only from a native function or callback.
 * Calling this function multiple times in a native function must always produce the same checksum.
 * If returned checksums are different, a corruption must have occurred.
 * <p>
 * Please note that returning a non-zero checksum does not mean the MicroJvm virtual machine data has not been corrupted,
 * as it is not possible for the MicroJvm virtual machine to detect the complete memory integrity.<br>
 * MicroJvm virtual machine internal structures allowed to be modified by a native function are not taken into account for the checksum computation:
 * <ul>
 * <li>basetype fields in Java objects or content of Java arrays of base type,</li>
 * <li>internal structures modified by a <code>LLMJVM</code> function call (set a pending Java exception, suspend or resume the Java thread, register a resource, ...).</li>
 * </ul>
 * <p>
 * This function affects performance and should only be used for debug purpose
 * <p>
 * A typical use of this API is to verify that a native implementation does not corrupt the internal structures:
 * <pre>
 * void Java_com_mycompany_MyClass_myNativeFunction(void) {
 * 		int32_t crcBefore = LLMJVM_checkIntegrity();
 * 		myNativeFunctionDo();
 *      int32_t crcAfter = LLMJVM_checkIntegrity();
 *      if(crcBefore != crcAfter){
 *      	// Corrupted MicroJVM virtual machine internal structures
 *      	while(1);
 *      }
 * }
 * <pre>
 * 
 * @return a 32 bits integrity checksum if the integrity check has run without errors, or <code>0</code> if an error has been detected.
 */
int32_t LLMJVM_checkIntegrity(void);

/**
 * Checks if the given Java array is immortal or not.
 * <p>
 * An immortal Java array can be used safely from another native task, an interrupt
 * handler or a DMA.
 * <p>
 * A non-immortal Java array can be used only during the execution of an SNI native or an
 * SNI callback. A reference to a non-immortal Java array must not be kept in C between to
 * SNI native executions.
 * 
 * @param javaArray is a Java array retrieved from an SNI native arguments.
 * This pointer reference the first element of an array and not the header of the object.
 * 
 * @return 1 if the given Java array is immortal or null, otherwise
 * returns 0.
 */
uint8_t LLMJVM_isImmortalArray(void* javaArray);

/**
 * If <code>ioException</code> is false:<br>
 * Throws a new <code>ej.sni.NativeException</code> after the end of the current native
 * method or SNI callback. <code>NativeException</code> class is a subclass of
 * <code>java.lang.RuntimeException</code>. <code>NativeException</code> instances are
 * unchecked exceptions, they do not need to be declared in the native method throws
 * clause.
 * <p>
 * If <code>ioException</code> is true:<br>
 * Throws a new <code>ej.sni.NativeIOException</code> after the end of the current native
 * method or SNI callback. <code>NativeIOException</code> class is a subclass of
 * <code>java.io.IOException</code>. <code>NativeIOException</code> instances are checked
 * exceptions, they need to be declared in the native method throws clause (e.g.,
 * <code>throws IOException</code>).
 * If the native method declaration is not compatible with <code>NativeIOException</code>
 * (i.e., the throws clause does not specify <code>NativeIOException</code> or one of its
 * superclasses) then a code>NativeException</code> is thrown instead.
 * <p>
 * If an SNI callback must be executed after the end of the current native method then it
 * will be executed first and then the exception will be thrown.
 * <p>
 * Calling this function while an exception is already pending will replace the previous
 * exception.
 * 
 * @param errorCode is a value that can be retrieved in Java using the
 * <code>Native(IO)Exception.getErrorCode()</code> method.
 * 
 * @param message is a null-terminated string that will be used to
 * generate the message returned by <code>Native(IO)Exception.getMessage()</code>. It may be
 * null.
 * 
 * @param ioException if true throws a NativeIOException, if false throws a NativeException.
 * 
 * @return {@link LLMJVM_E_OK}  on success. Returns {@link LLMJVM_E_ERROR} if the function is called outside of the VM Task.
 */
int32_t LLMJVM_throwNativeException(int32_t errorCode, void* message, uint8_t ioException);

/**
 * Returns 1 if an exception is pending in the current Java thread (i.e., a
 * successful call to {@link LLMJVM_throwNativeException} or
 * {@link #throwNativeIOException(int, Ram)} has been made prior to this call), returns
 * 0 otherwise.
 */
uint8_t LLMJVM_isExceptionPending(void);

/**
 * Clears any pending exception for the current Java thread.
 * 
 * @return {@link LLMJVM_E_OK}  on success. Returns {@link LLMJVM_E_ERROR} if the function is called outside of the VM Task.
 */
int32_t LLMJVM_clearPendingException(void);

/**
 * This function must be called when a native resource (file, socket, ...) has been created to register
 * it in the virtual machine.
 * <p>
 * The virtual machine keeps track of the native resources to prevent any memory leak when the virtual
 * machine or an application is stopped.
 * <p>
 * In a Multi-Sandbox environment with KF profile enabled, the given <code>close</code> function is
 * called when the application that has created the resource is stopped. If the resource has been created
 * by the Kernel or if KF profile is disabled, the <code>close</code> function is called when the virtual
 * machine stops.
 * <p>
 * To unregister the given native resource, call {@link LLMJVM_unregisterResource}.
 * <p>
 * A native resource is uniquely identified by the pair <code>resource</code>, <code>close</code>. Several
 * native resources with the same <code>resource</code> value can be registered as long as <code>close</code>
 * is different.
 * <p>
 * If a native resource is registered several times with the same <code>resource</code> and <code>close</code>
 * values, this method returns {@link LLMJVM_E_ILLEGAL_ARGUMENT} (unless it has been unregistered before a
 * second registration).
 * <p>
 * This function can be called once per native call. Calling this function several time within the same native
 * will return {@link LLMJVM_E_ERROR}.
 * <p>
 * If an error occurs while registering the resource, then the <code>close</code> function is called and an
 * exception is thrown when returning in Java.
 * 
 * @param resource the created native resource to register.
 * 
 * @param close the function called if the virtual machine wants to automatically close the resource.
 * 
 * @param getDescription the function called if the virtual machine wants to retrieve a description of the
 * native resource. This pointer function may be null.
 * 
 * @return {@link LLMJVM_E_OK} on success. Returns {@link LLMJVM_E_ILLEGAL_ARGUMENT} if <code>close</code> is
 * null or if the given native resource has already been registered. Returns {@link LLMJVM_E_ERROR} if the
 * function is not called within the virtual machine task or if this function has been called several time in the
 * same native.
 */
int32_t LLMJVM_registerResource(void* resource, void* close, void* getDescription);

/**
 * This function must be called when a native resource previously registered with {@link LLMJVM_registerResource}
 * has been reclaimed. It will unregister the given native resource so the virtual machine won't call later
 * the <code>close</code> function.
 * <p>
 * This method does not call the <code>close</code> function. It is up to the caller to call the appropriate function to close
 * the resource.
 * <p>
 * The <code>close</code> parameter is used to uniquely identify the registered resource. If the given native
 * resource is not found,  this method returns {@link LLMJVM_E_ILLEGAL_ARGUMENT}.
 * 
 * @param resource a resource previously registered with {@link LLMJVM_registerResource}.
 * 
 * @param close the <code>close</code> argument given to {@link LLMJVM_registerResource} when the native
 * resource has been registered.
 * 
 * @return {@link LLMJVM_E_OK} on success. Returns {@link LLMJVM_E_ILLEGAL_ARGUMENT} if the given native resource
 * is not registered. Returns {@link LLMJVM_E_ERROR} if the function is not called within the virtual machine task.
 */
int32_t LLMJVM_unregisterResource(void* resource, void* close);

/**
 * Registers a scoped native resource.
 * <p>
 * These native resources are similar to standard native resources but they consume less memory, they are faster
 * and they are closed automatically when the native returns back to Java. This function should be used when
 * a resource is allocated in a native and released in a callback of the same native. This will prevent memory
 * leaks that can occur when a thread is suspended using <code>SNI_suspendCurrentJavaThreadWithCallback()</code>
 * and the virtual machine or the current application is stopped.
 * <p>
 * In a Multi-Sandbox environment with KF profile enabled, the given <code>close</code> function is called
 * when the application that has created the scoped native resource is stopped. If the scoped native resource
 * has been created by the Kernel or if KF profile is disabled, the <code>close</code> function is called when
 * the virtual machine stops.
 * <p>
 * To unregister the given scoped native resource, call {@link LLMJVM_unregisterScopedResource}.
 * <p>
 * If several scoped native resources are registered within the same native context (or subsequent callback),
 * then this function returns {@link LLMJVM_E_ERROR} (unless the previously registered scoped native resource
 * has been unregistered).
 * 
 * @param resource the created scoped native resource to register.
 * 
 * @param close the function called if the virtual machine wants to automatically close the resource.
 * 
 * @param getDescription the function called if the virtual machine wants to retrieve a description of the
 * scoped native resource. This pointer function may be null.
 * 
 * @return {@link LLMJVM_E_OK} on success. Returns {@link LLMJVM_E_ILLEGAL_ARGUMENT} if <code>close</code> is
 * null. Returns {@link LLMJVM_E_ERROR} if the function is not called within the virtual machine task or if this
 * function has been called several time in the same native context.
 */
int32_t LLMJVM_registerScopedResource(void* resource, void* close, void* getDescription);

/**
 * Unregisters the scoped native resource previously registered with {@link LLMJVM_registerScopedResource}.
 * <p>
 * After a call to this function, a new scoped native resource can be registered.
 * <p>
 * Usually this function does not need to be called because the registered scoped native resource is automatically
 * closed and unregistered when the native returns back to Java.
 * 
 * @return {@link LLMJVM_E_OK} on success. Returns {@link LLMJVM_E_ERROR} if the function is not called within
 * the virtual machine task or if no scoped native resource has been previously registered.
 */
int32_t LLMJVM_unregisterScopedResource(void);

/**
 * Gets the information of the scoped native resource previously registered with {@link LLMJVM_registerScopedResource}.
 * 
 * @param resourcePtr output parameter. This function will store in this pointer the <code>resource</code>
 * value previously given to {@link LLMJVM_registerScopedResource}. May be null.
 * 
 * @param closePtr output parameter. This function will store in this pointer the <code>close</code>
 * value previously given to {@link LLMJVM_registerScopedResource}. May be null.
 * 
 * @param getDescriptionPtr output parameter. This function will store in this pointer the <code>getDescription</code>
 * value previously given to {@link LLMJVM_registerScopedResource}. May be null.
 * 
 * @return {@link LLMJVM_E_OK} on success. Returns {@link LLMJVM_E_ERROR} if the function is not called within
 * the virtual machine task or if no scoped native resource has been previously registered.
 */
int32_t LLMJVM_getScopedResource(void* resourcePtr, void* closePtr, void* getDescriptionPtr);

/**
 * Gets the Java resource data identified by the given path.
 * 
 * @param path string that contains the path. It must start with '/'.
 * 
 * @param pathLength length of the path string.
 * 
 * @param resource structure where the information of the resource are stored.
 * 
 * @return {@link LLMJVM_E_OK} on success. Returns {@link LLMJVM_E_ERROR} if the resource is not found.
 * Returns {@link LLMJVM_E_ILLEGAL_ARGUMENT} if pathLength is lower than 1 or resource is null.
 */
int32_t LLMJVM_getJavaResource(void* path, int32_t pathLength, void* resource);

/**
 * Returns the ID of the current Java thread.
 * This function should be called within the virtual machine
 * task.
 * Returns {@link LLMJVM_E_ERROR} if this function is not called within the
 * virtual machine task.
 */
int32_t LLMJVM_getCurrentJavaThreadID(void);

/**
 * Causes the current Java thread to pause its Java execution after the end
 * of the current native method. This function is not blocking.
 * The current Java thread will resume its execution after the reception
 * of an external event or after <code>timeout</code> milliseconds.
 * <p>
 * If a resume has been done on this thread before calling this function, the thread
 * is not paused.
 * <p>
 * The result of calling this method several times during the same native execution
 * is unpredictable.
 * <p>
 * If a callback is specified, then this callback is called before resuming the Java execution.
 * 
 * @param timeout duration in milliseconds of the pause. If
 * <code>timeout</code> is zero, then time is not taken into consideration and the
 * thread simply waits until resumed.
 * @param callback method to call when the timeout is reached or the {@link #sendExternalEvent(VMEngine)} method is
 * 			called on the current Java thread. May be null.
 * @param callbackSuspendArg argument that can be retrieved in the callback using {@link LLMJVM_getCallbackArgs}. May be null.
 * @param interruptible if true the suspend can be interrupted with a call to Thread.interrupt(), otherwise the suspend cannot be interrupted.
 * 
 * @return {@link LLMJVM_E_ERROR} if the method has been called outside of the VM Task.
 * Returns {@link LLMJVM_E_OK} if the pause is effective (i.e., the thread will suspend its execution
 * at the end of the current native).
 * Returns {@link LLMJVM_E_INTERRUPTED} if <code>interruptible</code> is true and an interrupt is pending (see Thread.interrupt()).
 */
int32_t LLMJVM_suspendCurrentJavaThread(int64_t timeout, void* callback, void* callbackSuspendArg, uint8_t interruptible);

/**
 * Causes the current Java thread to call Thread.yield() after the end
 * of the current native method and then to call the given callback when it will
 * be scheduled again.
 * <p>
 * The result of calling this method several times during the same native execution
 * is unpredictable.
 * 
 * @param callback method to call when the thread will be scheduled. May be null.
 * @param callbackArg argument that can be retrieved from the callback using {@link LLMJVM_getCallbackArgs}. May be null.
 * 
 * @return {@link LLMJVM_E_ERROR} if the method has been called outside of the VM Task.
 * Returns {@link LLMJVM_E_OK} on success.
 */
int32_t LLMJVM_javaThreadYield(void* callback, void* callbackArg);

/**
 * This method can be called from inside a SNI callback to retrieve arguments given to the callback with
 * {@link LLMJVM_javaThreadYield},  {@link LLMJVM_suspendCurrentJavaThread} or
 * {@link LLMJVM_resumeJavaThread}.
 * 
 * @param suspendArgPtr Out parameter. Pointer to the argument (void**) that will be filled-in by this method with the <code>callbackSuspendArg</code>
 * value given to {@link LLMJVM_suspendCurrentJavaThread} or <code>callbackArg</code> value given to
 * {@link LLMJVM_javaThreadYield}. May be null.
 * @param resumeArgPtr Out parameter. Pointer to the argument (void**) that will be filled-in by this method with the <code>callbackResumeArg</code>
 * value given to {@link LLMJVM_resumeJavaThread}. May be null.
 * 
 * @return {@link LLMJVM_E_ERROR} if the method has been called outside of the VM Task, otherwise returns {@link LLMJVM_E_OK}.
 */
int32_t LLMJVM_getCallbackArgs(void* suspendArgPtr, void* resumeArgPtr);

/**
 * Resume the given Java thread if it is suspended.
 * If the Java thread is not paused, this resume stays pending.
 * Next call of SNI_suspendCurrentJavaThread() will return immediately.
 * 
 * @param javaThreadID is the ID of the Java thread to resume.
 * @param callbackResumeArg argument that can be retrieved from the callback using {@link LLMJVM_getCallbackArgs}. May be null.
 * 
 * @return {@link LLMJVM_E_ERROR} if the given Java thread ID is invalid, otherwise returns {@link LLMJVM_E_OK}.
 */
int32_t LLMJVM_resumeJavaThread(int32_t javaThreadID, void* callbackResumeArg);

/**
 * Tests whether the given Java thread has been resumed while it was not suspended.
 * <p>
 * The pending resume flag is set when calling {@link LLMJVM_resumeJavaThread}. It is cleared when calling
 * {@link LLMJVM_suspendCurrentJavaThread},
 * or when the Java thread actually resumes its execution. The pending resume flag is
 * not modified by a timeout that occurs during a pause.
 * <p>
 * The pending resume flag of the given Java thread is unaffected by this method.
 * <p>
 * If the given Java thread ID is invalid, this method does nothing and returns false.
 * 
 * @param javaThreadID ID of the Java thread to test. The Java thread ID can be
 * retrieved using {@link LLMJVM_getCurrentJavaThreadID} function.
 * 
 * @return 1 if the pending resume flag of the given Java thread is set,
 * 0 otherwise.
 */
uint8_t LLMJVM_isResumePending(int32_t javaThreadID);

/**
 * Clears the pending resume flag of the current Java thread (see {@link LLMJVM_isResumePending}).
 * <p>
 * If this method is not called within the virtual machine task, it does nothing and returns false.
 * 
 * @return 1 if the pending resume flag of the current Java thread was set before
 * clearing it, 0 otherwise.
 */
uint8_t LLMJVM_clearCurrentJavaThreadPendingResumeFlag(void);

#ifdef __cplusplus
	}
#endif
#endif
