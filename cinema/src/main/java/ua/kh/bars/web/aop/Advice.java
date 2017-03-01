package ua.kh.bars.web.aop;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.authentication.AnonymousAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ModelAttribute;
import ua.kh.bars.entities.User;
import ua.kh.bars.services.UserAccountService;
import ua.kh.bars.services.UserService;

/**
 * Created by green on 4/3/16.
 */

@ControllerAdvice(basePackages = "ua.kh.bars.web")
public class Advice {

    @Autowired
    UserService userService;

    @Autowired
    UserAccountService userAccountService;

    // Just found an interesting way to set attribute for all views.
    // This code runs first.
    @ModelAttribute
    public void addAttributes(Model model) {
        Authentication authentication = SecurityContextHolder.getContext().getAuthentication();
        if (!(authentication instanceof AnonymousAuthenticationToken)) {
            UserDetails customUser = (UserDetails) authentication.getPrincipal();
            String userName = customUser.getUsername();
            User user = userService.findByUsername(userName);
            int prepaid = userAccountService.getPrepaid(user);
            model.addAttribute("prepaid", prepaid);
        }
    }
}
