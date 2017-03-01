package ua.kh.bars.services.Impl;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import ua.kh.bars.entities.User;
import ua.kh.bars.services.UserService;

import java.util.HashSet;
import java.util.Set;

/**
 * Created by green on 3/22/16.
 */

@Service
@Transactional
public class UserDetailsServiceImpl implements UserDetailsService {

    @Autowired
    private UserService userService;

    @Override
    public UserDetails loadUserByUsername(String username)
            throws UsernameNotFoundException {
        User user = userService.findByUsername(username);
        Set<GrantedAuthority> roles = new HashSet<>();
        for(String roleName : user.getRoles().split(",")) {
            roles.add(new SimpleGrantedAuthority(roleName));
        }
        UserDetails userDetails =
                new org.springframework.security.core.userdetails.User(user.getUsername(),
                        user.getPassword(), user.getEnabled(), true, true, true,
                        roles);
        return userDetails;
    }
}
