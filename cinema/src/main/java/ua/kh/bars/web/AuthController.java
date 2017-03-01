package ua.kh.bars.web;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.propertyeditors.CustomDateEditor;
import org.springframework.security.authentication.encoding.ShaPasswordEncoder;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.WebDataBinder;
import org.springframework.web.bind.annotation.InitBinder;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import ua.kh.bars.entities.User;
import ua.kh.bars.services.UserService;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by green on 3/22/16.
 */
@Controller
public class AuthController {

    @Autowired
    UserService userService;

    @Autowired
    ShaPasswordEncoder shaPasswordEncoder;

    @RequestMapping(value = "/login", method = RequestMethod.GET)
    public String loginPage(Model model) {
        return "auth/login";
    }

    @RequestMapping(value = "/login-error", method = RequestMethod.GET)
    public String loginErrorPage(Model model) {
        model.addAttribute("loginError", true);
        return "auth/login";
    }

    @RequestMapping(value = "/registration", method = RequestMethod.GET)
    public String registrationPage(Model model) {
        model.addAttribute("user", new User());
        return "auth/registration";
    }

    @RequestMapping(value = "/registration", method = RequestMethod.POST)
    public String postUser(@ModelAttribute User user, Model model) {
        user.setRoles("ROLE_USER");
        user.setEnabled(true);
        user.setPassword(shaPasswordEncoder.encodePassword(user.getPassword(), ""));
        User savedUser = userService.save(user);
        model.addAttribute("message", "Now login with your credentials");
        return "auth/login";
    }

    @InitBinder
    public void initBinder(WebDataBinder binder){
        binder.registerCustomEditor(Date.class,
                new CustomDateEditor(new SimpleDateFormat("yyyy-MM-dd"), true, 10));
    }


}
