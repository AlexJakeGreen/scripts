package ua.kh.bars.config;

import org.springframework.security.web.context.AbstractSecurityWebApplicationInitializer;

/**
 * Created by green on 3/22/16.
 */

// Registers the DelegatingFilterProxy to use the springSecurityFilterChain before any other registered Filter.
public class SecurityInit extends AbstractSecurityWebApplicationInitializer {

}
