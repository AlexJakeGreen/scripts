package ua.kh.bars.web;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import ua.kh.bars.entities.User;
import ua.kh.bars.services.UserService;

import java.util.List;

/**
 * Created by green on 3/20/16.
 */


@Controller
@RequestMapping("/search")
public class SearchController {

    @Autowired
    UserService userService;

    @RequestMapping(method = RequestMethod.POST)
    public String search(@RequestParam("searchValue") String searchValue,
                         Model model) {
        List<User> users = userService.findUsersByName(searchValue);
        model.addAttribute("users", users);
        return "users/index";
    }

}

